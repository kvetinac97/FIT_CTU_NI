import os
import sys

import streamlit as st

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import script
import helper as hp
import st_sidebar
import time

# functions
with st.spinner():
    if 'training_set' not in st.session_state:
        st.session_state.training_set = []

    if 'test_set' not in st.session_state:
        st.session_state.test_set = []

    if len(st.session_state.training_set) == 0:
        script.loadDataset(st.session_state.training_set, st.session_state.test_set)

# sidebar
st_sidebar.sidebar(st.session_state.training_set)

# page content

st.title('Test accuracy')
started = time.perf_counter()

with st.spinner('Calculating accuracies'):
    test_length = len(st.session_state.test_set)
    predictions = []
    for test_idx in range(test_length):
        predictions.append(
            hp.nearestClass(hp.getNeighbours(
                st.session_state.training_set, 
                st.session_state.test_set[test_idx], 
                [
                    st.session_state.spread_weight,
                    st.session_state.centroid_weight,
                    st.session_state.area_moments_weight,
                    st.session_state.area_moments_dev_weight,
                    st.session_state.moments_weight,
                    st.session_state.mfcc_weight,
                    st.session_state.mfcc_time_weight
                ],
                st.session_state.dtw_width,
                st.session_state.k_nearest
            )))

    # Calculate accuracy
    correct_tries = 0
    for test_idx in range(test_length):
        if st.session_state.test_set[test_idx][-1] == predictions[test_idx][0][0]:
            correct_tries += 1

    st.write(f"Accuracy {(100.0 * correct_tries / test_length):.2f}%")
    st.write(f"Elapsed {(time.perf_counter() - started):.2f}s")

    table = [    
        {
            "Genre": st.session_state.test_set[i][-1],
            "Classified as": predictions[i][0][0],
            "Probability": f"{(predictions[i][0][1] * 100.0):.0f}%"
        } for i in range(len(st.session_state.test_set))
    ]
    
    st.table(table)
