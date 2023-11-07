/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.Context;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.RecyclerView;

import com.bumptech.glide.RequestManager;

import org.chromium.hns_news.mojom.HnsNewsController;
import org.chromium.hns_news.mojom.Channel;
import org.chromium.hns_news.mojom.FeedSearchResultItem;
import org.chromium.hns_news.mojom.Publisher;
import org.chromium.hns_news.mojom.PublisherType;
import org.chromium.hns_news.mojom.UserEnabled;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.hns_news.HnsNewsUtils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

public class HnsNewsPreferencesTypeAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
    private HnsNewsController mHnsNewsController;
    private HnsNewsPreferencesListener mHnsNewsPreferencesListener;
    private HnsNewsPreferencesSearchType mHnsNewsPreferencesSearchType;
    private Context mContext;
    private HashMap<String, Integer> mChannelIcons = new HashMap<>();
    private List<Channel> mChannelList;
    private List<Publisher> mPublisherList;
    private List<FeedSearchResultItem> mFeedSearchResultItemList;
    private HashMap<String, String> mFeedSearchResultItemFollowMap;
    private RequestManager mGlide;
    private String mHnsNewsPreferencesType;
    private String mSearchUrl;

    private static int TYPE_SECTION = 1;
    private static int TYPE_ITEMS = 2;

    private static final int ONE_ITEM_SPACE = 1;
    private static final int TWO_ITEMS_SPACE = 2;

    public HnsNewsPreferencesTypeAdapter(Context context,
            HnsNewsPreferencesListener hnsNewsPreferencesListener,
            HnsNewsPreferencesSearchType hnsNewsPreferencesSearchType,
            HnsNewsController hnsNewsController, RequestManager glide,
            String hnsNewsPreferencesType, List<Channel> channelList,
            List<Publisher> publisherList) {
        mContext = context;
        mHnsNewsController = hnsNewsController;
        mHnsNewsPreferencesSearchType = hnsNewsPreferencesSearchType;
        mHnsNewsPreferencesListener = hnsNewsPreferencesListener;
        mGlide = glide;
        mHnsNewsPreferencesType = hnsNewsPreferencesType;
        mPublisherList = publisherList;
        mChannelList = channelList;
        if (mChannelList.size() > 0) {
            mChannelIcons = HnsNewsUtils.getChannelIcons();
        }
    }

    @Override
    public void onBindViewHolder(@NonNull RecyclerView.ViewHolder holder, int position) {
        if (holder instanceof NewsPreferencesSectionViewHolder) {
            NewsPreferencesSectionViewHolder sectionHolder =
                    (NewsPreferencesSectionViewHolder) holder;
            if (position == 0 && mChannelList.size() > 0) {
                sectionHolder.tvSection.setText(
                        mContext.getResources().getString(R.string.channels));
            } else {
                sectionHolder.tvSection.setText(
                        mContext.getResources().getString(R.string.sources));
            }

        } else if (holder instanceof NewsPreferencesViewHolder) {
            NewsPreferencesViewHolder viewHolder = (NewsPreferencesViewHolder) holder;
            if ((mHnsNewsPreferencesType.equalsIgnoreCase(
                         HnsNewsPreferencesType.PopularSources.toString())
                        || mHnsNewsPreferencesType.equalsIgnoreCase(
                                HnsNewsPreferencesType.Suggestions.toString()))
                    && position < mPublisherList.size()) {
                setSource(position, viewHolder, mPublisherList.get(position));

            } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                               HnsNewsPreferencesType.Following.toString())) {
                if (mChannelList.size() > 0 && position < mChannelList.size()) {
                    setChannel(position, viewHolder, mChannelList.get(position));
                } else if (mPublisherList.size() > 0 && position - mChannelList.size() >= 0
                        && (position - mChannelList.size()) < mPublisherList.size()) {
                    setSource(position, viewHolder,
                            mPublisherList.get(position - mChannelList.size()));
                }

            } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                               HnsNewsPreferencesType.Search.toString())) {
                if (mChannelList.size() > 0 && position < getChannelItemsCount()) {
                    setChannel(position, viewHolder, mChannelList.get(position - ONE_ITEM_SPACE));
                } else if (position == getItemCount() - ONE_ITEM_SPACE
                        && (mHnsNewsPreferencesSearchType
                                        == HnsNewsPreferencesSearchType.SearchUrl
                                || mHnsNewsPreferencesSearchType
                                        == HnsNewsPreferencesSearchType.GettingFeed
                                || mHnsNewsPreferencesSearchType
                                        == HnsNewsPreferencesSearchType.NotFound)) {
                    setSearchUrl(position, viewHolder);
                } else if (position - getChannelItemsCount() - ONE_ITEM_SPACE >= 0
                        && position - getChannelItemsCount() - ONE_ITEM_SPACE
                                < mPublisherList.size()) {
                    setSource(position, viewHolder,
                            mPublisherList.get(position - getChannelItemsCount() - ONE_ITEM_SPACE));
                } else if (mHnsNewsPreferencesSearchType
                        == HnsNewsPreferencesSearchType.NewSource) {
                    int resultPosition = 0;
                    if (mChannelList.size() > 0) {
                        resultPosition = mChannelList.size() + ONE_ITEM_SPACE;
                    }
                    resultPosition += mPublisherList.size() + ONE_ITEM_SPACE;

                    setFeedSearchResultItem(position - resultPosition, viewHolder);
                }

            } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                               HnsNewsPreferencesType.Channels.toString())
                    && position < getChannelItemsCount()) {
                setChannel(position, viewHolder, mChannelList.get(position));
            }
        }
    }

    private void setSearchUrl(int position, NewsPreferencesViewHolder viewHolder) {
        viewHolder.imagePublisher.setVisibility(View.GONE);
        viewHolder.imageChannel.setVisibility(View.GONE);

        if (mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.SearchUrl
                || mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.GettingFeed) {
            viewHolder.name.setText(mSearchUrl);
            viewHolder.subtitle.setVisibility(View.GONE);
            viewHolder.btnFollow.setVisibility(View.VISIBLE);
            if (mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.GettingFeed) {
                displayFollowButton(true, R.string.getting_feeds, viewHolder);
            } else {
                displayFollowButton(false, R.string.get_feeds, viewHolder);
            }

            viewHolder.btnFollow.setOnClickListener(view -> {
                if (mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.SearchUrl) {
                    mHnsNewsPreferencesSearchType = HnsNewsPreferencesSearchType.GettingFeed;
                    notifyItemChanged(position);
                    mHnsNewsPreferencesListener.findFeeds(mSearchUrl);
                }
            });
        } else if (mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.NotFound) {
            viewHolder.name.setText(
                    mContext.getResources().getString(R.string.no_feed_found, mSearchUrl));
            viewHolder.btnFollow.setVisibility(View.GONE);
            viewHolder.subtitle.setVisibility(View.GONE);
        }
    }

    private void setFeedSearchResultItem(int position, NewsPreferencesViewHolder viewHolder) {
        if (position >= 0 && position < mFeedSearchResultItemList.size()) {
            FeedSearchResultItem feedSearchResultItem = mFeedSearchResultItemList.get(position);
            viewHolder.name.setText(feedSearchResultItem.feedTitle);
            if (feedSearchResultItem.feedUrl != null && feedSearchResultItem.feedUrl.url != null
                    && feedSearchResultItem.feedUrl.url.length() > 0) {
                viewHolder.subtitle.setText(feedSearchResultItem.feedUrl.url);
                viewHolder.subtitle.setVisibility(View.VISIBLE);
            } else {
                viewHolder.subtitle.setVisibility(View.GONE);
            }
            viewHolder.imagePublisher.setVisibility(View.GONE);
            viewHolder.imageChannel.setVisibility(View.GONE);
            viewHolder.btnFollow.setVisibility(View.VISIBLE);
            boolean isFollow = mFeedSearchResultItemFollowMap != null
                    && mFeedSearchResultItemFollowMap.containsKey(feedSearchResultItem.feedUrl.url)
                    && mFeedSearchResultItemFollowMap.get(feedSearchResultItem.feedUrl.url) != null;
            if (!isFollow) {
                displayFollowButton(false, R.string.follow, viewHolder);
            } else {
                displayFollowButton(true, R.string.unfollow, viewHolder);
            }
            viewHolder.btnFollow.setOnClickListener(view -> {
                if (!isFollow) {
                    mHnsNewsPreferencesListener.subscribeToNewDirectFeed(
                            position, feedSearchResultItem.feedUrl, true);
                } else if (mFeedSearchResultItemFollowMap.containsKey(
                                   feedSearchResultItem.feedUrl.url)) {
                    String publisherId =
                            mFeedSearchResultItemFollowMap.get(feedSearchResultItem.feedUrl.url);
                    mHnsNewsPreferencesListener.updateFeedSearchResultItem(
                            position, feedSearchResultItem.feedUrl.url, publisherId);

                    mHnsNewsPreferencesListener.onPublisherPref(
                            publisherId, UserEnabled.DISABLED);
                }
            });
        }
    }

    private void setDirectSource(
            int position, NewsPreferencesViewHolder viewHolder, Publisher publisher) {
        if (publisher.feedSource != null && publisher.feedSource.url != null
                && publisher.feedSource.url.length() > 0) {
            viewHolder.subtitle.setText(publisher.feedSource.url);
            viewHolder.subtitle.setVisibility(View.VISIBLE);
        } else {
            viewHolder.subtitle.setVisibility(View.GONE);
        }

        if (publisher.userEnabledStatus == UserEnabled.DISABLED) {
            displayFollowButton(false, R.string.follow, viewHolder);
        } else {
            displayFollowButton(true, R.string.unfollow, viewHolder);
        }

        viewHolder.btnFollow.setOnClickListener(view -> {
            if (publisher.userEnabledStatus == UserEnabled.DISABLED) {
                publisher.userEnabledStatus = UserEnabled.ENABLED;
            } else {
                publisher.userEnabledStatus = UserEnabled.DISABLED;
            }

            notifyItemChanged(position);
            if (publisher.userEnabledStatus == UserEnabled.ENABLED) {
                mHnsNewsPreferencesListener.subscribeToNewDirectFeed(
                        position, publisher.feedSource, false);
            } else {
                mHnsNewsPreferencesListener.onPublisherPref(
                        publisher.publisherId, publisher.userEnabledStatus);
            }
        });
    }

    private void setSource(
            int position, NewsPreferencesViewHolder viewHolder, Publisher publisher) {
        viewHolder.name.setText(publisher.publisherName);

        viewHolder.imagePublisher.setVisibility(View.VISIBLE);
        viewHolder.imageChannel.setVisibility(View.GONE);
        if (publisher.backgroundColor != null) {
            try {
                viewHolder.imagePublisher.setBackgroundTintList(
                        ColorStateList.valueOf(Color.parseColor(publisher.backgroundColor)));
            } catch (Exception exception) {
                // Incase backgroundColor string is not proper hex string
                viewHolder.imagePublisher.setBackgroundTintList(
                        ColorStateList.valueOf(Color.TRANSPARENT));
            }
        } else {
            viewHolder.imagePublisher.setBackgroundTintList(
                    ColorStateList.valueOf(Color.TRANSPARENT));
        }
        if (mHnsNewsController != null && publisher.coverUrl != null) {
            mHnsNewsController.getImageData(publisher.coverUrl, imageData -> {
                if (imageData != null) {
                    mGlide.load(imageData).fitCenter().into(viewHolder.imagePublisher);
                } else {
                    mGlide.clear(viewHolder.imagePublisher);
                }
            });
        } else {
            mGlide.clear(viewHolder.imagePublisher);
        }

        viewHolder.btnFollow.setVisibility(View.VISIBLE);

        if (publisher.type == PublisherType.DIRECT_SOURCE) {
            setDirectSource(position, viewHolder, publisher);
        } else {
            viewHolder.subtitle.setVisibility(View.GONE);

            if (publisher.userEnabledStatus == UserEnabled.ENABLED) {
                displayFollowButton(true, R.string.unfollow, viewHolder);
            } else {
                displayFollowButton(false, R.string.follow, viewHolder);
            }

            viewHolder.btnFollow.setOnClickListener(view -> {
                if (publisher.userEnabledStatus == UserEnabled.ENABLED) {
                    publisher.userEnabledStatus = UserEnabled.DISABLED;
                } else {
                    publisher.userEnabledStatus = UserEnabled.ENABLED;
                }
                notifyItemChanged(position);
                mHnsNewsPreferencesListener.onPublisherPref(
                        publisher.publisherId, publisher.userEnabledStatus);
            });
        }
    }

    private void setChannel(int position, NewsPreferencesViewHolder viewHolder, Channel channel) {
        viewHolder.name.setText(channel.channelName);
        viewHolder.subtitle.setVisibility(View.GONE);
        viewHolder.imagePublisher.setVisibility(View.GONE);
        viewHolder.imageChannel.setVisibility(View.VISIBLE);

        if (mChannelIcons.containsKey(channel.channelName)) {
            viewHolder.imageChannel.setImageResource(mChannelIcons.get(channel.channelName));
        } else {
            viewHolder.imageChannel.setImageResource(R.drawable.ic_channel_default);
        }

        List<String> subscribedLocalesList =
                new ArrayList<>(Arrays.asList(channel.subscribedLocales));

        viewHolder.btnFollow.setVisibility(View.VISIBLE);
        if (subscribedLocalesList.contains(HnsNewsUtils.getLocale())) {
            displayFollowButton(true, R.string.unfollow, viewHolder);
        } else {
            displayFollowButton(false, R.string.follow, viewHolder);
        }

        viewHolder.btnFollow.setOnClickListener(view -> {
            boolean isSubscribed = subscribedLocalesList.contains(HnsNewsUtils.getLocale());
            isSubscribed = !isSubscribed;
            if (isSubscribed) {
                subscribedLocalesList.add(HnsNewsUtils.getLocale());
            } else {
                subscribedLocalesList.remove(HnsNewsUtils.getLocale());
            }
            String[] subscribedLocales = new String[subscribedLocalesList.size()];
            channel.subscribedLocales = subscribedLocalesList.toArray(subscribedLocales);
            notifyItemChanged(position);

            mHnsNewsPreferencesListener.onChannelSubscribed(position, channel, isSubscribed);
        });
    }

    private void displayFollowButton(
            boolean isFollowing, int textId, NewsPreferencesViewHolder holder) {
        if (isFollowing) {
            holder.btnFollow.setBackgroundResource(R.drawable.hns_news_settings_unfollow_bg);
            holder.btnText.setTextColor(
                    ContextCompat.getColor(mContext, R.color.news_settings_unfollow_color));
        } else {
            holder.btnFollow.setBackgroundResource(R.drawable.blue_48_rounded_bg);
            holder.btnText.setTextColor(ContextCompat.getColor(mContext, android.R.color.white));
        }

        if (mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.GettingFeed) {
            holder.btnLoading.setVisibility(View.VISIBLE);
        } else {
            holder.btnLoading.setVisibility(View.GONE);
        }

        holder.btnText.setText(textId);
    }

    public void setItems(List<Channel> channelList, List<Publisher> publisherList, String searchUrl,
            HnsNewsPreferencesSearchType hnsNewsPreferencesSearchType,
            HashMap<String, String> feedSearchResultItemFollowMap) {
        mChannelList = channelList;
        mPublisherList = publisherList;
        mSearchUrl = searchUrl;
        mHnsNewsPreferencesSearchType = hnsNewsPreferencesSearchType;
        mFeedSearchResultItemFollowMap = feedSearchResultItemFollowMap;

        if (mChannelList.size() > 0 && mChannelIcons.size() == 0) {
            mChannelIcons = HnsNewsUtils.getChannelIcons();
        }

        int itemsInserted = getChannelItemsCount() + getPublisherItemsCount();

        if (mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.SearchUrl) {
            itemsInserted += ONE_ITEM_SPACE;
        }

        notifyItemRangeInserted(0, itemsInserted);
    }

    public void setFindFeeds(List<FeedSearchResultItem> feedSearchResultItemList,
            HnsNewsPreferencesSearchType hnsNewsPreferencesSearchType) {
        int startPosition = getItemCount() - 1;
        notifyItemRemoved(startPosition);

        mFeedSearchResultItemList = feedSearchResultItemList;
        mHnsNewsPreferencesSearchType = hnsNewsPreferencesSearchType;
        if (hnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.NewSource) {
            notifyItemRangeInserted(startPosition, feedSearchResultItemList.size());
        } else if (hnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.NotFound) {
            notifyItemRangeInserted(startPosition, ONE_ITEM_SPACE);
        }
    }

    public void setFeedSearchResultItemFollowMap(
            int position, HashMap<String, String> feedSearchResultItemFollowMap) {
        mFeedSearchResultItemFollowMap = feedSearchResultItemFollowMap;
        int itemPosition = 0;
        if (mChannelList.size() > 0) {
            itemPosition = mChannelList.size() + ONE_ITEM_SPACE;
        }
        itemPosition += mPublisherList.size() + ONE_ITEM_SPACE;
        itemPosition += position;

        notifyItemChanged(itemPosition);
    }

    @Override
    public int getItemCount() {
        if (mHnsNewsPreferencesType.equalsIgnoreCase(
                    HnsNewsPreferencesType.PopularSources.toString())
                || mHnsNewsPreferencesType.equalsIgnoreCase(
                        HnsNewsPreferencesType.Suggestions.toString())) {
            return mPublisherList.size();

        } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                           HnsNewsPreferencesType.Channels.toString())) {
            return mChannelList.size();
        } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                           HnsNewsPreferencesType.Following.toString())) {
            return mChannelList.size() + mPublisherList.size();
        } else if (mHnsNewsPreferencesType.equalsIgnoreCase(
                           HnsNewsPreferencesType.Search.toString())) {
            int count = getPublisherItemsCount() + getChannelItemsCount();

            if (mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.SearchUrl
                    || mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.GettingFeed
                    || mHnsNewsPreferencesSearchType == HnsNewsPreferencesSearchType.NotFound) {
                if (mPublisherList.size() == 0) {
                    count += ONE_ITEM_SPACE;
                }
                count += ONE_ITEM_SPACE;
            } else if (mHnsNewsPreferencesSearchType
                    == HnsNewsPreferencesSearchType.NewSource) {
                if (mPublisherList.size() == 0) {
                    count += ONE_ITEM_SPACE;
                }
                count += mFeedSearchResultItemList.size();
            }

            return count;
        } else {
            return 0;
        }
    }

    @Override
    public int getItemViewType(int position) {
        if (mHnsNewsPreferencesType.equalsIgnoreCase(
                    HnsNewsPreferencesType.Search.toString())) {
            if (position == 0) {
                return TYPE_SECTION;
            } else if (mChannelList.size() > 0
                    && mChannelList.size() + ONE_ITEM_SPACE == position) {
                return TYPE_SECTION;
            } else {
                return TYPE_ITEMS;
            }
        } else {
            return TYPE_ITEMS;
        }
    }

    private int getChannelItemsCount() {
        int count = mChannelList.size();

        if (mChannelList.size() > 0) {
            count += ONE_ITEM_SPACE;
        }
        return count;
    }

    private int getPublisherItemsCount() {
        int count = mPublisherList.size();

        if (mPublisherList.size() > 0) {
            count += ONE_ITEM_SPACE;
        }
        return count;
    }

    @NonNull
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view;
        if (viewType == TYPE_SECTION) {
            view = LayoutInflater.from(parent.getContext())
                           .inflate(R.layout.item_news_settings_section, parent, false);
            return new NewsPreferencesSectionViewHolder(view);
        } else {
            view = LayoutInflater.from(parent.getContext())
                           .inflate(R.layout.item_news_settings, parent, false);
            return new NewsPreferencesViewHolder(view);
        }
    }

    public static class NewsPreferencesSectionViewHolder extends RecyclerView.ViewHolder {
        TextView tvSection;

        NewsPreferencesSectionViewHolder(View itemView) {
            super(itemView);
            this.tvSection = (TextView) itemView.findViewById(R.id.tv_section);
        }
    }

    public static class NewsPreferencesViewHolder extends RecyclerView.ViewHolder {
        ImageView imagePublisher;
        ImageView imageChannel;
        TextView name;
        TextView subtitle;
        TextView btnText;
        ProgressBar btnLoading;
        LinearLayout btnFollow;

        NewsPreferencesViewHolder(View itemView) {
            super(itemView);
            this.imagePublisher = (ImageView) itemView.findViewById(R.id.iv_publisher);
            this.imageChannel = (ImageView) itemView.findViewById(R.id.iv_channel);
            this.name = (TextView) itemView.findViewById(R.id.tv_name);
            this.subtitle = (TextView) itemView.findViewById(R.id.tv_subtitle);
            this.btnText = (TextView) itemView.findViewById(R.id.btn_text);
            this.btnLoading = (ProgressBar) itemView.findViewById(R.id.btn_loading);
            this.btnFollow = (LinearLayout) itemView.findViewById(R.id.btn_follow);
        }
    }
}
