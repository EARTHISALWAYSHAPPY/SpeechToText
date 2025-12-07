import sounddevice as sd
import numpy as np
from faster_whisper import WhisperModel

SAMPLE_RATE = 16000
RECORD_SECS = 5
MODEL_SIZE = "base"

def create_model():
    try:
        print("Trying CUDA...")
        model = WhisperModel(MODEL_SIZE, device="cuda", compute_type="float16")
        print("Using CUDA ✅")
        return model
    except Exception as e:
        print("CUDA failed:", e)
        print("Falling back to CPU...")
        model = WhisperModel(MODEL_SIZE, device="cpu", compute_type="int8")
        print("Using CPU ✅")
        return model

model = create_model()

while True:
    try:
        audio = sd.rec(
            int(SAMPLE_RATE * RECORD_SECS),
            samplerate=SAMPLE_RATE,
            channels=1,
            dtype="float32",
        )
        sd.wait()

        samples = audio.flatten()

        segments, _ = model.transcribe(samples, language="th")
        text = "".join(seg.text for seg in segments).strip()
        print(text if text else "-")

    except KeyboardInterrupt:
        break
    except Exception as e:
        print("Error:", e)
