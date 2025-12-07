from vosk import Model, KaldiRecognizer
import sounddevice as sd
import queue
import json
import time
import numpy as np

SAMPLE_RATE = 16000
BLOCK_SIZE = 800

MODEL_PATH = "Proto/vosk-model-en-us-0.22-lgraph"

model = Model(MODEL_PATH)
rec = KaldiRecognizer(model, SAMPLE_RATE)

audio_q = queue.Queue()
last_voice_time = time.time()

SILENCE_THRESHOLD = 0.010
SILENCE_DURATION = 0.38


def audio_callback(indata, frames, time_info, status):
    if status:
        print(status)
    audio_q.put(bytes(indata))


def main():
    global last_voice_time
    print("READY. Speak now.")

    with sd.RawInputStream(
        samplerate=SAMPLE_RATE,
        blocksize=BLOCK_SIZE,
        dtype="int16",
        channels=1,
        callback=audio_callback,
    ):
        while True:
            data_bytes = audio_q.get()

            arr = np.frombuffer(data_bytes, dtype=np.int16)
            volume = np.abs(arr / 32768.0).mean()
            if volume > SILENCE_THRESHOLD:
                last_voice_time = time.time()

            rec.AcceptWaveform(data_bytes)

            if time.time() - last_voice_time > SILENCE_DURATION:
                final_text = json.loads(rec.FinalResult()).get("text", "").strip()
                if final_text:
                    print("\n[SENTENCE]", final_text)
                rec.Reset()
                last_voice_time = time.time()
                continue

            partial = json.loads(rec.PartialResult()).get("partial", "").strip()
            if partial:
                print("[PARTIAL]", partial)

            time.sleep(0.005)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nEXIT")
