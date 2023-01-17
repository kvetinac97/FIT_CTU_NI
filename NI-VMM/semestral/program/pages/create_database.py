import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import streamlit as st
import script

# sidebar

# page content
st.title("Create database")

with st.spinner("Building database..."):
    script.buildDatabase()

st.success("Database built")
