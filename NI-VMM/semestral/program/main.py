import streamlit as st
from tempfile import NamedTemporaryFile
import helper as hp
import os
import matplotlib.pyplot as plt
import st_sidebar


# functions

def loadDatabase():
    return hp.loadDataset('db.dat')


with st.spinner(text="Loading database..."):
    dataset = loadDatabase()


@st.cache(show_spinner=False)
def checkUploadedFile():
    if uploaded_file is not None:
        with NamedTemporaryFile(suffix=".wav", delete=False) as temp:
            temp.write(uploaded_file.getvalue())
            temp.seek(0)

        features = hp.getFeatures(temp.name)
        features.append('')
        st.session_state.features = features
        st.session_state.file_name = uploaded_file.name
        os.remove(temp.name)
        print(temp.name)


# sidebar

uploaded_file = st.sidebar.file_uploader("Upload .wav file", type="wav", help="Upload file you want to classify")
st_sidebar.sidebar(dataset)

# page content
st.title('Music Genre Classifier')

with st.spinner(text="Processing uploaded file"):
    try:
        checkUploadedFile()

        if 'features' in st.session_state:

            st.write(f"File {st.session_state.file_name}")

            result = hp.nearestClass(hp.getNeighbours(
                dataset,
                st.session_state.features,
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
            ))

            labels = [prediction[0] for prediction in result]
            values = [prediction[1] for prediction in result]
            explode = [0.05] + [0 for _ in range(len(result) - 1)]

            fig, ax = plt.subplots()
            ax.pie(values, explode=explode, labels=labels, autopct='%1.f%%')
            st.pyplot(fig)

        else:
            st.info('Upload a WAV file first', icon="ℹ️")
    except ValueError as e:
        st.error(e, icon="🚨")
