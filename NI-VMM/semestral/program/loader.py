import random
import string
import os
import subprocess
import xml.etree.ElementTree as ET
import scipy.io.wavfile as wav
from python_speech_features import mfcc
import numpy as np


def loadMpegDescriptors(file):
    mpeg = subprocess.run(["java", "-jar", "mpeg.jar", file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if mpeg.returncode != 0:
        raise ValueError("Mpeg descriptors could not be extracted from the file")

    parser = ET.XMLParser(encoding='UTF-8')
    tree = ET.ElementTree(ET.fromstring(mpeg.stdout, parser=parser))

    root = tree.getroot()
    xmlns = "{urn:mpeg:mpeg7:schema:2001}"
    xsi = "{http://www.w3.org/2001/XMLSchema-instance}"

    centroid = root.find(
        f'.//{xmlns}AudioDescriptor[@{xsi}type="AudioSpectrumCentroidType"]/{xmlns}SeriesOfScalar/{xmlns}Raw')
    spread = root.find(
        f'.//{xmlns}AudioDescriptor[@{xsi}type="AudioSpectrumSpreadType"]/{xmlns}SeriesOfScalar/{xmlns}Raw')

    centroid_values = np.fromstring(centroid.text, sep=" ")
    spread_values = np.fromstring(spread.text, sep=" ")

    return np.average(centroid_values), np.average(spread_values)


def loadJAudioDescriptors(file):
    xmlName = ''.join(random.choice(string.ascii_lowercase) for _ in range(15))  # random output file name
    mpeg = subprocess.run(["java", "-jar", "jAudio.jar", "-s", "settings.xml", xmlName, file],
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if mpeg.returncode != 0:
        raise ValueError("jAudio descriptors could not be extracted from the file")

    parser = ET.XMLParser(encoding='UTF-8')
    tree = ET.ElementTree(ET.parse(f"{xmlName}FV.xml", parser=parser))
    os.remove(f"{xmlName}FV.xml")
    os.remove(f"{xmlName}FK.xml")

    root = tree.getroot()

    area_moments = []
    area_moments_dev = []
    moments = []

    for feature in root.findall('.//feature'):
        if feature.find('name').text == 'Area Method of Moments Overall Average':
            for value in feature.iter('v'):
                area_moments.append(float(value.text))

        if feature.find('name').text == 'Area Method of Moments Overall Standard Deviation':
            for value in feature.iter('v'):
                area_moments_dev.append(float(value.text))

        if feature.find('name').text == 'Method of Moments Overall Average':
            for value in feature.iter('v'):
                moments.append(float(value.text))

    return area_moments[-1], area_moments_dev[-1], moments[-1]


def loadMfccCoefficients(file):
    (rate, sig) = wav.read(file)
    mfcc_features = mfcc(sig, rate, winlen=0.020, appendEnergy=False, nfft=1024)
    return mfcc_features.mean(0).flatten(), mfcc_features[:, 0][:256]
