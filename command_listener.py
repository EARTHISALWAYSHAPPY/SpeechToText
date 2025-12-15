# command_listener.py
import speech_recognition as sr

def listen_for_command():
    r = sr.Recognizer()
    r.dynamic_energy_threshold = True
    r.pause_threshold = 0.8

    with sr.Microphone() as source:
        print("🎙️ Listening for command...")
        audio = r.listen(source, timeout=5, phrase_time_limit=6)

    try:
        text = r.recognize_google(audio, language="th")
        print("[COMMAND TEXT]", text)
        return text
    except sr.UnknownValueError:
        print("[STT] Could not understand")
    except sr.RequestError as e:
        print("[STT ERROR]", e)

    return None
