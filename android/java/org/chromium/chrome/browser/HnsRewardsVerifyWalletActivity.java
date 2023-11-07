/**
 * Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Spanned;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.HnsActivity;

import java.util.Locale;

public class HnsRewardsVerifyWalletActivity extends Activity {
    private String walletType = HnsRewardsNativeWorker.getInstance().getExternalWalletType();
    private String walletTypeString;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.verify_wallet_activity);

        walletTypeString = walletType.equals(HnsWalletProvider.UPHOLD)
                ? getResources().getString(R.string.uphold)
                : getResources().getString(R.string.bitflyer);

        setTextForWalletTypes();
        SetVerifyWalletBtnClickHandler();
        SetUpholdLinkHandler();
    }

    void setTextForWalletTypes() {
        TextView verifyWalletBenefitText1 = (TextView) findViewById(R.id.verify_wallet_benefit_1);
        verifyWalletBenefitText1.setText(walletType.equals(HnsWalletProvider.UPHOLD)
                        ? getResources().getString(R.string.verify_wallet_uphold_benefits1)
                        : getResources().getString(R.string.verify_wallet_bitflyer_benefits1));
        TextView verifyWalletBenefitText2 = (TextView) findViewById(R.id.verify_wallet_benefit_2);
        verifyWalletBenefitText2.setText(walletType.equals(HnsWalletProvider.UPHOLD)
                        ? getResources().getString(R.string.verify_wallet_uphold_benefits2)
                        : getResources().getString(R.string.verify_wallet_bitflyer_benefits2));
        TextView verifyWalletBenefitText3 = (TextView) findViewById(R.id.verify_wallet_benefit_3);
        verifyWalletBenefitText3.setText(walletType.equals(HnsWalletProvider.UPHOLD)
                        ? getResources().getString(R.string.verify_wallet_uphold_benefits3)
                        : getResources().getString(R.string.verify_wallet_bitflyer_benefits3));

        TextView verifyWalletProviderNoteText =
                (TextView) findViewById(R.id.verify_wallet_provider_note_txt);
        verifyWalletProviderNoteText.setText(String.format(
                getResources().getString(R.string.verify_wallet_provider_note1), walletTypeString));

        TextView verifyWalletHnsNoteText =
                (TextView) findViewById(R.id.verify_wallet_hns_note_txt);
        verifyWalletHnsNoteText.setText(String.format(
                getResources().getString(R.string.verify_wallet_hns_note1), walletTypeString));
    }

    void SetVerifyWalletBtnClickHandler() {
        Button btnVerifyWallet = (Button)findViewById(R.id.verify_wallet_btn);
        btnVerifyWallet.setOnClickListener((View v) -> {
            String login_url = getIntent().getStringExtra(HnsRewardsExternalWallet.LOGIN_URL);
            Intent intent = new Intent();
            intent.putExtra(HnsActivity.OPEN_URL, login_url);
            setResult(RESULT_OK, intent);
            finish();
        });
    }

    @SuppressLint("ClickableViewAccessibility")
    void SetUpholdLinkHandler() {
        TextView uphold_link = (TextView)findViewById(R.id.service_provider_txt);
        final String part1 = getResources().getString(R.string.verify_wallet_service_note);
        final String part2 = walletTypeString;
        final String built_service_str = String.format(Locale.US, "%s <b>%s</b>", part1, part2);
        Spanned toInsert = HnsRewardsHelper.spannedFromHtmlString(built_service_str);
        uphold_link.setText(toInsert);

        uphold_link.setOnTouchListener(
                (View view, MotionEvent motionEvent) -> {
                    boolean event_consumed = false;
                    if (motionEvent.getAction() == MotionEvent.ACTION_DOWN) {
                        int offset = uphold_link.getOffsetForPosition(
                                motionEvent.getX(), motionEvent.getY());

                        if (HnsRewardsHelper.subtextAtOffset(built_service_str, part2, offset) ){
                            Intent intent = new Intent();
                            intent.putExtra(
                                    HnsActivity.OPEN_URL, HnsWalletProvider.UPHOLD_ORIGIN_URL);
                            setResult(RESULT_OK, intent);
                            finish();
                            event_consumed = true;
                        }
                    }
                    return event_consumed;
        });
    }
}
