/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.hns_news;

import android.app.Activity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.bumptech.glide.RequestManager;

import org.chromium.hns_news.mojom.HnsNewsController;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.hns_news.models.FeedItemsCard;

import java.util.concurrent.CopyOnWriteArrayList;

public class HnsNewsAdapterFeedCard
        extends RecyclerView.Adapter<HnsNewsAdapterFeedCard.ViewHolder> {
    private LayoutInflater mInflater;
    private Activity mActivity;
    private View mView;
    private RequestManager mGlide;

    public CopyOnWriteArrayList<FeedItemsCard> mNewsItems;
    private FeedItemsCard mNewsItem;
    private ViewHolder mHolder;
    private final String TAG = "BN";

    private HnsNewsController mHnsNewsController;

    public HnsNewsAdapterFeedCard(Activity activity, RequestManager glide,
            CopyOnWriteArrayList<FeedItemsCard> newsItems,
            HnsNewsController hnsNewsController) {
        this.mInflater = LayoutInflater.from(activity);
        this.mActivity = activity;
        this.mNewsItems = newsItems;
        this.mHnsNewsController = hnsNewsController;
        this.mGlide = glide;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                            .inflate(R.layout.hns_news_row, parent, false);

        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        LinearLayout.LayoutParams params1;
        if (mNewsItems != null) {
            FeedItemsCard newsItem = mNewsItems.get(position);
            holder.linearLayout.removeAllViews();
            try {
                if (mHnsNewsController != null) {
                    new CardBuilderFeedCard(mHnsNewsController, mGlide, holder.linearLayout,
                            mActivity, position, newsItem, newsItem.getCardType());
                }

            } catch (Exception e) {
                Log.e(TAG, "crashinvestigation onBindViewHolder e: " + e);
            }
        }
    }

    @Override
    public int getItemCount() {
        return mNewsItems.size();
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        LinearLayout linearLayout;

        ViewHolder(View itemView) {
            super(itemView);
            this.linearLayout = (LinearLayout) itemView.findViewById(R.id.card_layout);
        }
    }

    public FeedItemsCard getItem(int id) {
        return mNewsItems.get(id);
    }
}
