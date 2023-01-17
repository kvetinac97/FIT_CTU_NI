import streamlit as st


def sidebar(dataset):
    if 'spread_weight' not in st.session_state:
        st.session_state.spread_weight = 0.0

    if 'centroid_weight' not in st.session_state:
        st.session_state.centroid_weight = 0.0

    if 'area_moments_weight' not in st.session_state:
        st.session_state.area_moments_weight = 0.0

    if 'area_moments_dev_weight' not in st.session_state:
        st.session_state.area_moments_dev_weight = 0.0

    if 'moments_weight' not in st.session_state:
        st.session_state.moments_weight = 0.0

    if 'mfcc_weight' not in st.session_state:
        st.session_state.mfcc_weight = 1.0

    if 'mfcc_time_weight' not in st.session_state:
        st.session_state.mfcc_time_weight = 0.0

    if 'k_nearest' not in st.session_state:
        st.session_state.k_nearest = 5

    if 'dtw_width' not in st.session_state:
        st.session_state.dtw_width = 5

    max_k_nearest = len(dataset)

    if st.session_state.k_nearest > max_k_nearest:
        st.session_state.k_nearest = max_k_nearest

    st.sidebar.header("Weights")
    st.sidebar.slider("Spectrum spread", key="spread_weight")
    st.sidebar.slider("Spectral centroid", key="centroid_weight")
    st.sidebar.slider("Area of moments", key="area_moments_weight")
    st.sidebar.slider("Area of moments standard deviation", key="area_moments_dev_weight")
    st.sidebar.slider("Method of moments", key="moments_weight")
    st.sidebar.slider("MFCC mean", key="mfcc_weight")
    st.sidebar.slider("MFCC time line", key="mfcc_time_weight")

    st.sidebar.header("Recognition settings")
    st.sidebar.slider("Count of neighbours", min_value=1, max_value=max_k_nearest, key="k_nearest")
    st.sidebar.slider("Width of the window for DTW", max_value=40, key="dtw_width")
