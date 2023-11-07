/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.SharedPreferences;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffColorFilter;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.appcompat.widget.SwitchCompat;
import androidx.core.content.ContextCompat;

import com.airbnb.lottie.LottieAnimationView;
import com.airbnb.lottie.LottieProperty;
import com.airbnb.lottie.model.KeyPath;

import org.chromium.base.HnsPreferenceKeys;
import org.chromium.base.ContextUtils;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;
import org.chromium.hns_news.mojom.HnsNewsController;
import org.chromium.hns_news.mojom.Channel;
import org.chromium.hns_news.mojom.Publisher;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.hns_news.HnsNewsControllerFactory;
import org.chromium.chrome.browser.hns_news.HnsNewsUtils;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.night_mode.GlobalNightModeStateProviderHolder;
import org.chromium.chrome.browser.preferences.HnsPrefServiceBridge;
import org.chromium.chrome.browser.preferences.SharedPreferencesManager;
import org.chromium.chrome.browser.util.HnsConstants;
import org.chromium.components.browser_ui.settings.FragmentSettingsLauncher;
import org.chromium.components.browser_ui.settings.SettingsLauncher;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;

import java.util.List;

public class HnsNewsPreferencesV2 extends HnsPreferenceFragment
        implements HnsNewsPreferencesDataListener, ConnectionErrorHandler,
                   FragmentSettingsLauncher {
    public static final String PREF_SHOW_OPTIN = "show_optin";

    private LinearLayout mParentLayout;
    private LinearLayout mOptinLayout;
    private SwitchCompat mSwitchShowNews;
    private TextView mTvSearch;
    private TextView mTvFollowingCount;
    private Button mBtnTurnOnNews;
    private Button mBtnLearnMore;
    private View mLayoutSwitch;
    private View mDivider;
    private View mLayoutPopularSources;
    private View mLayoutSuggestions;
    private View mLayoutChannels;
    private View mLayoutFollowing;

    private boolean mIsSuggestionAvailable;
    private boolean mIsChannelAvailable;
    private boolean mIsPublisherAvailable;
    private HnsNewsController mHnsNewsController;

    // SettingsLauncher injected from main Settings Activity.
    private SettingsLauncher mSettingsLauncher;

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.hns_news_settings, container, false);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        if (getActivity() != null) {
            getActivity().setTitle(R.string.hns_news_title);
        }

        super.onActivityCreated(savedInstanceState);

        initHnsNewsController();

        View view = getView();
        if (view != null) {
            mParentLayout = (LinearLayout) view.findViewById(R.id.layout_parent);
            mOptinLayout = (LinearLayout) view.findViewById(R.id.layout_optin_card);
            mSwitchShowNews = (SwitchCompat) view.findViewById(R.id.switch_show_news);
            mDivider = view.findViewById(R.id.divider);
            mLayoutSwitch = view.findViewById(R.id.layout_switch);
            mBtnTurnOnNews = (Button) view.findViewById(R.id.btn_turn_on_news);
            mBtnLearnMore = (Button) view.findViewById(R.id.btn_learn_more);
            mTvSearch = (TextView) view.findViewById(R.id.tv_search);
            mTvFollowingCount = (TextView) view.findViewById(R.id.tv_following_count);
            mLayoutPopularSources = (View) view.findViewById(R.id.layout_popular_sources);
            mLayoutSuggestions = (View) view.findViewById(R.id.layout_suggestions);
            mLayoutChannels = (View) view.findViewById(R.id.layout_channels);
            mLayoutFollowing = (View) view.findViewById(R.id.layout_following);

            setData();
            onClickViews();
        }
    }

    private void setData() {
        if (!GlobalNightModeStateProviderHolder.getInstance().isInNightMode()
                && getView() != null) {
            LottieAnimationView lottieAnimationVIew =
                    (LottieAnimationView) getView().findViewById(R.id.animation_view);

            try {
                lottieAnimationVIew.addValueCallback(new KeyPath("newspaper", "**"),
                        LottieProperty.COLOR_FILTER,
                        frameInfo
                        -> new PorterDuffColorFilter(ContextCompat.getColor(getActivity(),
                                                             R.color.news_settings_optin_color),
                                PorterDuff.Mode.SRC_ATOP));
            } catch (Exception exception) {
                // if newspaper keypath changed in animation json
            }
        }

        if (HnsNewsUtils.getLocale() != null
                && HnsNewsUtils.getSuggestionsPublisherList().size() > 0) {
            mIsSuggestionAvailable = true;
        }

        boolean isNewsEnable = HnsNewsUtils.shouldDisplayNews();
        mSwitchShowNews.setChecked(isNewsEnable);
        onShowNewsToggle(isNewsEnable);
    }

    @Override
    public void onResume() {
        super.onResume();

        if (HnsNewsUtils.getLocale() != null && mSwitchShowNews.isChecked()) {
            updateFollowerCount();

            if (!mIsSuggestionAvailable) {
                PostTask.postTask(TaskTraits.BEST_EFFORT, () -> {
                    if (mHnsNewsController != null) {
                        HnsNewsUtils.getSuggestionsSources(mHnsNewsController, this);
                    }
                });
            }
        }
    }

    private void onClickViews() {
        mBtnTurnOnNews.setOnClickListener(view -> { mSwitchShowNews.setChecked(true); });

        mBtnLearnMore.setOnClickListener(view -> {
            CustomTabActivity.showInfoPage(getActivity(), HnsConstants.HNS_NEWS_LEARN_MORE_URL);
        });

        mSwitchShowNews.setOnCheckedChangeListener((compoundButton, b) -> { onShowNewsToggle(b); });

        mTvSearch.setOnClickListener(
                view -> { openHnsNewsPreferencesDetails(HnsNewsPreferencesType.Search); });

        mLayoutPopularSources.setOnClickListener(view -> {
            openHnsNewsPreferencesDetails(HnsNewsPreferencesType.PopularSources);
        });

        mLayoutSuggestions.setOnClickListener(
                view -> { openHnsNewsPreferencesDetails(HnsNewsPreferencesType.Suggestions); });

        mLayoutChannels.setOnClickListener(
                view -> { openHnsNewsPreferencesDetails(HnsNewsPreferencesType.Channels); });

        mLayoutFollowing.setOnClickListener(view -> {
            if (HnsNewsUtils.getFollowingPublisherList().size() > 0
                    || HnsNewsUtils.getFollowingChannelList().size() > 0) {
                openHnsNewsPreferencesDetails(HnsNewsPreferencesType.Following);
            }
        });
    }

    private void onShowNewsToggle(boolean isEnable) {
        HnsPrefServiceBridge.getInstance().setShowNews(isEnable);

        SharedPreferencesManager.getInstance().writeBoolean(
                HnsPreferenceKeys.HNS_NEWS_PREF_SHOW_NEWS, isEnable);

        FrameLayout.LayoutParams parentLayoutParams = new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.WRAP_CONTENT);

        if (isEnable) {
            parentLayoutParams.gravity = Gravity.NO_GRAVITY;
            mParentLayout.setLayoutParams(parentLayoutParams);
            mOptinLayout.setVisibility(View.GONE);
            mLayoutSwitch.setVisibility(View.VISIBLE);
            mDivider.setVisibility(View.VISIBLE);
            if (HnsNewsUtils.getChannelIcons().size() == 0) {
                HnsNewsUtils.setChannelIcons();
            }
            if (HnsNewsUtils.getLocale() == null && mHnsNewsController != null) {
                HnsNewsUtils.getHnsNewsSettingsData(mHnsNewsController, this);
            } else {
                mTvSearch.setVisibility(View.VISIBLE);
                mLayoutPopularSources.setVisibility(View.VISIBLE);
                mLayoutChannels.setVisibility(View.VISIBLE);
                mLayoutFollowing.setVisibility(View.VISIBLE);
                updateFollowerCount();
            }

            HnsPrefServiceBridge.getInstance().setNewsOptIn(true);
            SharedPreferences.Editor sharedPreferencesEditor =
                    ContextUtils.getAppSharedPreferences().edit();
            sharedPreferencesEditor.putBoolean(HnsNewsPreferencesV2.PREF_SHOW_OPTIN, false);
            sharedPreferencesEditor.apply();

            if (mIsSuggestionAvailable) {
                mLayoutSuggestions.setVisibility(View.VISIBLE);
            }

        } else {
            parentLayoutParams.gravity = Gravity.CENTER_VERTICAL;
            mParentLayout.setLayoutParams(parentLayoutParams);
            mOptinLayout.setVisibility(View.VISIBLE);
            mLayoutSwitch.setVisibility(View.GONE);
            mDivider.setVisibility(View.GONE);
            mTvSearch.setVisibility(View.GONE);
            mLayoutPopularSources.setVisibility(View.GONE);
            mLayoutSuggestions.setVisibility(View.GONE);
            mLayoutChannels.setVisibility(View.GONE);
            mLayoutFollowing.setVisibility(View.GONE);
        }
    }

    private void openHnsNewsPreferencesDetails(
            HnsNewsPreferencesType hnsNewsPreferencesType) {
        Bundle fragmentArgs = new Bundle();
        fragmentArgs.putString(
                HnsConstants.HNS_NEWS_PREFERENCES_TYPE, hnsNewsPreferencesType.toString());
        mSettingsLauncher.launchSettingsActivity(
                getActivity(), HnsNewsPreferencesDetails.class, fragmentArgs);
    }

    private void initHnsNewsController() {
        if (mHnsNewsController != null) {
            return;
        }

        mHnsNewsController =
                HnsNewsControllerFactory.getInstance().getHnsNewsController(this);
    }

    private void updateFollowerCount() {
        List<Publisher> followingPublisherList = HnsNewsUtils.getFollowingPublisherList();
        List<Channel> followingChannelList = HnsNewsUtils.getFollowingChannelList();
        int followingCount = followingChannelList.size() + followingPublisherList.size();
        if (mLayoutFollowing != null && mTvFollowingCount != null) {
            mTvFollowingCount.setText(String.valueOf(followingCount));
            mLayoutFollowing.setVisibility(View.VISIBLE);
        }
    }

    @Override
    public void onChannelReceived() {
        if (mSwitchShowNews != null && mSwitchShowNews.isChecked()) {
            if (mLayoutChannels != null) {
                mLayoutChannels.setVisibility(View.VISIBLE);
            }

            mIsChannelAvailable = true;
            if (mIsPublisherAvailable) {
                if (mTvSearch != null) {
                    mTvSearch.setVisibility(View.VISIBLE);
                }
                updateFollowerCount();
            }
        }
    }

    @Override
    public void onPublisherReceived() {
        if (mSwitchShowNews != null && mSwitchShowNews.isChecked()) {
            if (mLayoutPopularSources != null) {
                mLayoutPopularSources.setVisibility(View.VISIBLE);
            }
            mIsPublisherAvailable = true;
            if (mIsChannelAvailable) {
                if (mTvSearch != null) {
                    mTvSearch.setVisibility(View.VISIBLE);
                }
                updateFollowerCount();
            }
        }
    }

    @Override
    public void onSuggestionsReceived() {
        if (mSwitchShowNews != null && mSwitchShowNews.isChecked()
                && HnsNewsUtils.getSuggestionsPublisherList().size() > 0) {
            if (mLayoutSuggestions != null) {
                mLayoutSuggestions.setVisibility(View.VISIBLE);
            }
        }
    }

    @Override
    public void setSettingsLauncher(SettingsLauncher settingsLauncher) {
        mSettingsLauncher = settingsLauncher;
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
