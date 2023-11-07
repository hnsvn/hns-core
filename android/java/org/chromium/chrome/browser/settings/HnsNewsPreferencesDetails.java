/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.URLUtil;

import androidx.appcompat.widget.SearchView;
import androidx.appcompat.widget.Toolbar;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.SimpleItemAnimator;

import com.bumptech.glide.Glide;

import org.chromium.base.HnsPreferenceKeys;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;
import org.chromium.hns_news.mojom.HnsNewsController;
import org.chromium.hns_news.mojom.Channel;
import org.chromium.hns_news.mojom.FeedSearchResultItem;
import org.chromium.hns_news.mojom.Publisher;
import org.chromium.hns_news.mojom.UserEnabled;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.hns_news.HnsNewsControllerFactory;
import org.chromium.chrome.browser.hns_news.HnsNewsUtils;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.util.HnsConstants;
import org.chromium.components.browser_ui.settings.SearchUtils;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.url.mojom.Url;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class HnsNewsPreferencesDetails extends HnsPreferenceFragment
        implements HnsNewsPreferencesListener, ConnectionErrorHandler {
    private RecyclerView mRecyclerView;

    private HnsNewsPreferencesTypeAdapter mAdapter;
    private HnsNewsController mHnsNewsController;
    private List<Publisher> mPublisherList;
    private String mHnsNewsPreferencesType;
    private String mSearch = "";
    private HashMap<String, String> mFeedSearchResultItemFollowMap = new HashMap<>();

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.hns_news_settings_details, container, false);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        initHnsNewsController();

        mRecyclerView = (RecyclerView) getView().findViewById(R.id.recyclerview);

        mHnsNewsPreferencesType =
                getArguments().getString(HnsConstants.HNS_NEWS_PREFERENCES_TYPE);
        setData();
    }

    private void setData() {
        List<Publisher> publisherList = new ArrayList<>();
        List<Channel> channelsList = new ArrayList<>();
        if (mHnsNewsPreferencesType.equalsIgnoreCase(
                    HnsNewsPreferencesType.PopularSources.toString())) {
            publisherList = HnsNewsUtils.getPopularSources();
            getActivity().setTitle(R.string.popular);
        } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                           HnsNewsPreferencesType.Suggestions.toString())) {
            publisherList = HnsNewsUtils.getSuggestionsPublisherList();
            getActivity().setTitle(R.string.suggestions);
        } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                           HnsNewsPreferencesType.Channels.toString())) {
            getActivity().setTitle(R.string.channels);
            channelsList = HnsNewsUtils.getChannelList();
        } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                           HnsNewsPreferencesType.Following.toString())) {
            getActivity().setTitle(R.string.following);
            publisherList = HnsNewsUtils.getFollowingPublisherList();
            channelsList = HnsNewsUtils.getFollowingChannelList();
        } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                           HnsNewsPreferencesType.Search.toString())) {
            getView().findViewById(R.id.search_divider).setVisibility(View.VISIBLE);

            Toolbar actionBar = getActivity().findViewById(R.id.action_bar);
            actionBar.setContentInsetsAbsolute(0, 0);
            actionBar.setContentInsetStartWithNavigation(0);
        }

        LinearLayoutManager linearLayoutManager =
                new LinearLayoutManager(getActivity(), LinearLayoutManager.VERTICAL, false);
        mRecyclerView.setLayoutManager(linearLayoutManager);
        mAdapter = new HnsNewsPreferencesTypeAdapter(getActivity(), this,
                HnsNewsPreferencesSearchType.Init, mHnsNewsController,
                Glide.with(getActivity()), mHnsNewsPreferencesType, channelsList, publisherList);
        mRecyclerView.setAdapter(mAdapter);

        if (mRecyclerView.getItemAnimator() != null) {
            RecyclerView.ItemAnimator itemAnimator = mRecyclerView.getItemAnimator();
            if (itemAnimator instanceof SimpleItemAnimator) {
                SimpleItemAnimator simpleItemAnimator = (SimpleItemAnimator) itemAnimator;
                simpleItemAnimator.setSupportsChangeAnimations(false);
            }
        }

        Drawable horizontalDivider = ContextCompat.getDrawable(
                getActivity(), R.drawable.hns_news_settings_list_divider);
        mRecyclerView.addItemDecoration(
                new HnsNewsSettingsDividerItemDecoration(horizontalDivider));
    }

    private void initHnsNewsController() {
        if (mHnsNewsController != null) {
            return;
        }

        mHnsNewsController =
                HnsNewsControllerFactory.getInstance().getHnsNewsController(this);
    }

    @Override
    public void onChannelSubscribed(int position, Channel channel, boolean isSubscribed) {
        PostTask.postTask(TaskTraits.BEST_EFFORT, () -> {
            if (mHnsNewsController != null) {
                newsChangeSource();
                mHnsNewsController.setChannelSubscribed(HnsNewsUtils.getLocale(),
                        channel.channelName, isSubscribed,
                        ((updatedChannel) -> { HnsNewsUtils.setFollowingChannelList(); }));
            }
        });
    }

    @Override
    public void onPublisherPref(String publisherId, int userEnabled) {
        PostTask.postTask(TaskTraits.BEST_EFFORT, () -> {
            if (mHnsNewsController != null) {
                newsChangeSource();
                mHnsNewsController.setPublisherPref(publisherId, userEnabled);
                HnsNewsUtils.setFollowingPublisherList();
            }
        });
    }

    @Override
    public void findFeeds(String url) {
        PostTask.postTask(TaskTraits.BEST_EFFORT, () -> {
            if (mHnsNewsController != null) {
                Url searchUrl = new Url();
                searchUrl.url = url;
                mHnsNewsController.findFeeds(searchUrl, ((results) -> {
                    if (!url.equals(mSearch)) return;

                    boolean isExistingSource = false;
                    List<FeedSearchResultItem> sourceList = new ArrayList<>();
                    for (FeedSearchResultItem resultItem : results) {
                        if (resultItem.feedUrl != null
                                && !HnsNewsUtils.searchPublisherForRss(resultItem.feedUrl.url)) {
                            sourceList.add(resultItem);
                        } else {
                            isExistingSource = true;
                        }
                    }
                    HnsNewsPreferencesSearchType hnsNewsPreferencesSearchType;
                    if (sourceList.size() > 0) {
                        hnsNewsPreferencesSearchType = HnsNewsPreferencesSearchType.NewSource;
                    } else if (isExistingSource) {
                        hnsNewsPreferencesSearchType =
                                HnsNewsPreferencesSearchType.Init; // ExistingSource;
                    } else {
                        hnsNewsPreferencesSearchType = HnsNewsPreferencesSearchType.NotFound;
                    }
                    mAdapter.setFindFeeds(sourceList, hnsNewsPreferencesSearchType);
                }));
            }
        });
    }

    @Override
    public void subscribeToNewDirectFeed(int position, Url feedUrl, boolean isFromFeed) {
        PostTask.postTask(TaskTraits.BEST_EFFORT, () -> {
            if (mHnsNewsController != null) {
                mHnsNewsController.subscribeToNewDirectFeed(
                        feedUrl, ((isValidFeed, isDuplicate, publishers) -> {
                            if (isValidFeed && publishers != null && publishers.size() > 0) {
                                newsChangeSource();
                                HnsNewsUtils.setPublishers(publishers);
                            }

                            if (publishers != null) {
                                for (Map.Entry<String, Publisher> entry : publishers.entrySet()) {
                                    Publisher publisher = entry.getValue();
                                    if (publisher.feedSource.url.equalsIgnoreCase(feedUrl.url)) {
                                        publisher.userEnabledStatus = UserEnabled.ENABLED;
                                        if (isFromFeed) {
                                            updateFeedSearchResultItem(position,
                                                    publisher.feedSource.url,
                                                    publisher.publisherId);
                                        } else {
                                            mAdapter.notifyItemChanged(position);
                                        }
                                        break;
                                    }
                                }
                            }
                        }));
            }
        });
    }

    @Override
    public void updateFeedSearchResultItem(int position, String url, String publisherId) {
        if (mFeedSearchResultItemFollowMap.containsKey(url)) {
            mFeedSearchResultItemFollowMap.remove(url);
        } else {
            mFeedSearchResultItemFollowMap.put(url, publisherId);
        }
        mAdapter.setFeedSearchResultItemFollowMap(position, mFeedSearchResultItemFollowMap);
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        MenuItem closeItem = menu.findItem(R.id.close_menu_id);
        if (closeItem != null) {
            closeItem.setVisible(false);
        }
        if (mHnsNewsPreferencesType.equalsIgnoreCase(
                    HnsNewsPreferencesType.Search.toString())) {
            inflater.inflate(R.menu.menu_hns_news_settings_search, menu);

            MenuItem searchItem = menu.findItem(R.id.menu_id_search);
            SearchView searchView = (SearchView) searchItem.getActionView();
            searchView.setMaxWidth(Integer.MAX_VALUE);
            searchView.setQueryHint(getActivity().getString(R.string.hns_news_settings_search));
            SearchUtils.initializeSearchView(searchItem, mSearch, getActivity(), (query) -> {
                boolean queryHasChanged = mSearch == null ? query != null && !query.isEmpty()
                                                          : !mSearch.equals(query);
                mSearch = query;
                if (queryHasChanged && mSearch.length() > 0) {
                    search();
                } else if (mSearch.length() == 0) {
                    mAdapter.notifyItemRangeRemoved(0, mAdapter.getItemCount());
                    mAdapter.setItems(new ArrayList<Channel>(), new ArrayList<Publisher>(), null,
                            HnsNewsPreferencesSearchType.Init, mFeedSearchResultItemFollowMap);
                }
            });
        }
    }

    private void search() {
        List<Channel> channelList = HnsNewsUtils.searchChannel(mSearch);
        List<Publisher> publisherList = HnsNewsUtils.searchPublisher(mSearch);
        String feedUrl = mSearch;
        String searchUrl = null;
        mFeedSearchResultItemFollowMap = new HashMap<>();
        HnsNewsPreferencesSearchType hnsNewsPreferencesSearchType =
                HnsNewsPreferencesSearchType.Init;

        if (feedUrl.contains(".")) {
            if (!feedUrl.contains("://")) {
                feedUrl = "https://" + feedUrl;
            }

            if (URLUtil.isValidUrl(feedUrl)) {
                searchUrl = feedUrl;

                hnsNewsPreferencesSearchType = HnsNewsPreferencesSearchType.SearchUrl;
            }
        }
        mSearch = searchUrl;
        mAdapter.notifyItemRangeRemoved(0, mAdapter.getItemCount());
        mAdapter.setItems(channelList, publisherList, searchUrl, hnsNewsPreferencesSearchType,
                mFeedSearchResultItemFollowMap);
        mRecyclerView.scrollToPosition(0);
    }

    public void newsChangeSource() {
        SharedPreferencesManager.getInstance().writeBoolean(
                HnsPreferenceKeys.HNS_NEWS_CHANGE_SOURCE, true);
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (mHnsNewsController != null) {
            mHnsNewsController.close();
        }
        mHnsNewsController = null;
        initHnsNewsController();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mHnsNewsController != null) {
            mHnsNewsController.close();
        }
    }
}
