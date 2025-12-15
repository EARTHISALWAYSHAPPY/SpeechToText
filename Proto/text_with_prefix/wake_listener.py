# wake_listener.py
from vosk import Model, KaldiRecognizer
import sounddevice as sd
import queue
import json
import time
import numpy as np
import re
from rapidfuzz import fuzz

SAMPLE_RATE = 16000
MODEL_PATH = "vosk-model-en-us-0.22-lgraph"

WAKE_WORD = "Ok Ras pie"
WAKE_SCORE_THRESHOLD = 80

SILENCE_THRESHOLD = 0.01
SILENCE_DURATION = 0.3


def normalize_wake(text):
    text = text.lower()
    text = re.sub(r"[^a-z\s]", "", text)
    words = [w for w in text.split() if w not in {"the", "a", "an"}]
    return " ".join(words)


def is_wake(text):
    score = fuzz.partial_ratio(text, WAKE_WORD)
    print(f"[WAKE SCORE] {score}")
    return score >= WAKE_SCORE_THRESHOLD


class WakeWordListener:
    def __init__(self):
        print("[INIT] Loading Vosk wake model...")
        self.model = Model(MODEL_PATH)
        self.rec = KaldiRecognizer(self.model, SAMPLE_RATE)

        self.audio_q = queue.Queue()
        self.last_voice_time = time.time()
        self.has_spoken = False

    def callback(self, indata, frames, time_info, status):
        self.audio_q.put(bytes(indata))

    def listen_for_wake(self):
        with sd.RawInputStream(
            samplerate=SAMPLE_RATE,
            blocksize=800,
            dtype="int16",
            channels=1,
            callback=self.callback
        ):
            print("🎧 Waiting for wake word...")

            while True:
                data = self.audio_q.get()
                arr = np.frombuffer(data, dtype=np.int16)
                vol = np.abs(arr / 32768.0).mean()

                if vol > SILENCE_THRESHOLD:
                    self.last_voice_time = time.time()
                    self.has_spoken = True

                self.rec.AcceptWaveform(data)

                if self.has_spoken and time.time() - self.last_voice_time > SILENCE_DURATION:
                    result = json.loads(self.rec.FinalResult())
                    raw = result.get("text", "")

                    self.rec.Reset()
                    self.has_spoken = False

                    if not raw:
                        continue

                    clean = normalize_wake(raw)
                    print("[WAKE RAW ]", raw)
                    print("[WAKE CLEAN]", clean)

                    if is_wake(clean):
                        print("🔓 WAKE DETECTED")
                        return