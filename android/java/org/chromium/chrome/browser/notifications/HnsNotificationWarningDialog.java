/**
 * Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.notifications;

import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.content.res.ResourcesCompat;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.HnsDialogFragment;
import org.chromium.chrome.browser.HnsRewardsHelper;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;

/**
 * This dialog is used to show different messages when notification permission is off and
 * if rewards or privacy is on OR both on
 * */
public class HnsNotificationWarningDialog extends HnsDialogFragment {
    public static final String NOTIFICATION_WARNING_DIALOG_TAG = "NotificationWarningDialog";

    public static final int FROM_LAUNCHED_HNS_SETTINGS = 1;
    public static final int FROM_LAUNCHED_HNS_ACTIVITY = 2;
    public static final int FROM_LAUNCHED_HNS_PANEL = 3;
    private static final String LAUNCHED_FROM = "launched_from";

    private TextView mTitleTextView;
    private TextView mDescriptionTextView;
    private Button mPrimaryButton;
    private int mLaunchedFrom;

    public interface DismissListener {
        void onDismiss();
    }
    private DismissListener mListener;
    public void setDismissListener(DismissListener listener) {
        mListener = listener;
    }

    @Override
    public void onDismiss(@NonNull DialogInterface dialog) {
        super.onDismiss(dialog);
        if (mListener != null && mLaunchedFrom == FROM_LAUNCHED_HNS_ACTIVITY)
            mListener.onDismiss();
    }

    public static HnsNotificationWarningDialog newInstance(int launchedFrom) {
        HnsNotificationWarningDialog fragment = new HnsNotificationWarningDialog();
        Bundle args = new Bundle();
        args.putInt(LAUNCHED_FROM, launchedFrom);
        fragment.setArguments(args);
        return fragment;
    }

    /**
     *  Should show dialog if any one is true
     *  1. No notification permission
     *  2. Notification permission is there but general or ads group is blocked
     *
     * if above any case is there and rewards / privacy / both enabled.
     * */
    public static boolean shouldShowNotificationWarningDialog(Context context) {
        if (!HnsPermissionUtils.hasNotificationPermission(context)) {
            return true;
        } else if (shouldShowRewardWarningDialog(context)
                || shouldShowPrivacyWarningDialog(context)) {
            return true;
        }
        return false;
    }

    public static boolean shouldShowRewardWarningDialog(Context context) {
        return HnsRewardsHelper.isRewardsEnabled()
                && HnsPermissionUtils.isHnsAdsNotificationPermissionBlocked(context);
    }

    public static boolean shouldShowPrivacyWarningDialog(Context context) {
        return isPrivacyReportsEnabled()
                && HnsPermissionUtils.isGeneralNotificationPermissionBlocked(context);
    }

    private static boolean shouldShowBothWarningDialog(Context context) {
        if (!HnsPermissionUtils.hasNotificationPermission(context)) {
            return true;
        }
        return shouldShowRewardWarningDialog(context) && shouldShowPrivacyWarningDialog(context);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.hns_notification_warning_dialog, container, false);
        if (getDialog() != null && getDialog().getWindow() != null) {
            getDialog().getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
            getDialog().getWindow().requestFeature(Window.FEATURE_NO_TITLE);
        }
        return view;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        init(view);
    }

    private void init(View view) {
        mTitleTextView = view.findViewById(R.id.notification_title_tv);
        mDescriptionTextView = view.findViewById(R.id.notification_description_tv);
        mPrimaryButton = view.findViewById(R.id.notification_warning_primary_button);
        updateTitleDescriptionText(view);
        clickOnPrimaryButton(view);
        clickOnCloseButton(view);
        clickOnNotNow(view);
    }

    public static boolean isPrivacyReportsEnabled() {
        return OnboardingPrefManager.getInstance().isHnsStatsEnabled();
    }

    private void updateTitleDescriptionText(View view) {
        if (getArguments() != null) {
            mLaunchedFrom = getArguments().getInt(LAUNCHED_FROM);
            if (mLaunchedFrom == FROM_LAUNCHED_HNS_ACTIVITY) {
                launchedFromHnsActivity(view);
            } else if (mLaunchedFrom == FROM_LAUNCHED_HNS_SETTINGS) {
                launchedFromHnsSettings(view);
                view.findViewById(R.id.btn_not_now).setVisibility(View.GONE);
            } else if (mLaunchedFrom == FROM_LAUNCHED_HNS_PANEL) {
                launchedFromHnsPanel(view);
            }
        }
    }

    private void launchedFromHnsPanel(View view) {
        ImageView icon = view.findViewById(R.id.warning_imageview);
        icon.setImageDrawable(
                ResourcesCompat.getDrawable(view.getResources(), R.drawable.ic_bell_icon, null));
        mTitleTextView.setText(R.string.enable_notifications_from_hns_to_earn_hns_rewards);
        mDescriptionTextView.setText(
                R.string.open_settings_and_turn_on_device_notifications_for_hns_ads);
        view.findViewById(R.id.btn_not_now).setVisibility(View.GONE);
        mPrimaryButton.setText(R.string.hns_open_system_sync_settings);
        mPrimaryButton.setBackground(ResourcesCompat.getDrawable(
                view.getResources(), R.drawable.blue_48_rounded_bg, null));
    }

    private void launchedFromHnsActivity(View view) {
        mPrimaryButton.setText(R.string.turn_on_hns_notifications);

        if (shouldShowBothWarningDialog(getContext())) {
            mTitleTextView.setText(R.string.notification_os_dialog_header_both_rewards_privacy);
            mDescriptionTextView.setText(
                    R.string.notification_os_dialog_description_both_rewards_privacy);
        } else if (shouldShowRewardWarningDialog(getContext())) {
            mTitleTextView.setText(R.string.notification_os_dialog_header_only_rewards);
            mDescriptionTextView.setText(R.string.notification_os_dialog_description_only_rewards);
        } else if (shouldShowPrivacyWarningDialog(getContext())) {
            mTitleTextView.setText(R.string.notification_os_dialog_header_only_privacy);
            mDescriptionTextView.setText(R.string.notification_os_dialog_description_only_privacy);
        }
    }

    private void launchedFromHnsSettings(View view) {
        mPrimaryButton.setText(R.string.got_it);

        if (shouldShowBothWarningDialog(getContext())) {
            mTitleTextView.setText(R.string.notification_hns_dialog_header_both_rewards_privacy);
            mDescriptionTextView.setText(
                    R.string.notification_hns_dialog_description_both_rewards_privacy);
        } else if (shouldShowRewardWarningDialog(getContext())) {
            mTitleTextView.setText(R.string.notification_hns_dialog_header_only_rewards);
            mDescriptionTextView.setText(
                    R.string.notification_hns_dialog_description_only_rewards);
        } else if (shouldShowPrivacyWarningDialog(getContext())) {
            mTitleTextView.setText(R.string.notification_hns_dialog_header_only_privacy);
            mDescriptionTextView.setText(
                    R.string.notification_hns_dialog_description_only_privacy);
        }
    }

    private void clickOnPrimaryButton(View view) {
        Button primaryButton = view.findViewById(R.id.notification_warning_primary_button);
        primaryButton.setOnClickListener(v -> {
            dismiss();
            HnsPermissionUtils.requestPermission(getActivity());
        });
    }

    private void clickOnCloseButton(View view) {
        ImageView btnClose = view.findViewById(R.id.notification_dialog_close);
        btnClose.setOnClickListener(v -> { dismiss(); });
    }

    private void clickOnNotNow(View view) {
        Button notNowButton = view.findViewById(R.id.btn_not_now);
        notNowButton.setOnClickListener(v -> { dismiss(); });
    }
}
