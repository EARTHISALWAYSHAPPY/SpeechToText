import speech_recognition as sr
import pyaudio

mymic = sr.Microphone(1)
Data = sr.Recognizer()

import speech_recognition as sr
import pyaudio

# sr.Microphone.list_microphone_names()
mymic = sr.Microphone(1)
Data = sr.Recognizer()
with mymic as source:
    while True:
        audio = Data.listen(source)
        try:
            print(Data.recognize_google(audio, language="th"))
        except:
            continue
