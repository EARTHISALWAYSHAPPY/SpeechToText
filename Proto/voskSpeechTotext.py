from vosk import Model, KaldiRecognizer
import sounddevice as sd
import queue
import json
import time
import numpy as np

SAMPLE_RATE = 16000
MODEL_PATH = "vosk-model-en-us-0.22-lgraph"

print("[INIT] Loading Vosk model...")
model = Model(MODEL_PATH)
rec = KaldiRecognizer(model, SAMPLE_RATE)

audio_q = queue.Queue()

# ---- ค่าปรับสำหรับการแยกประโยค ----
SILENCE_THRESHOLD = 0.010
SILENCE_DURATION  = 0.38
# ------------------------------------

last_voice_time = time.time()


def audio_callback(indata, frames, time_info, status):
    if status:
        print("[AUDIO WARNING]", status)

    # แก้จุดสำคัญ: indata เป็น cffi buffer → ใช้ bytes() ได้
    audio_q.put(bytes(indata))   # ส่ง raw bytes ไป main thread


with sd.RawInputStream(
    samplerate=SAMPLE_RATE,
    blocksize=800,
    dtype='int16',
    channels=1,
    callback=audio_callback
):
    print(">>> READY. Speak now.\n")

    while True:
        data_bytes = audio_q.get()

        # --- วัด volume เพื่อจับ silence ---
        arr = np.frombuffer(data_bytes, dtype=np.int16)
        volume = np.abs(arr / 32768.0).mean()

        if volume > SILENCE_THRESHOLD:
            last_voice_time = time.time()

        # ส่งข้อมูลให้ recognizer
        rec.AcceptWaveform(data_bytes)

        # --- silence detect เพื่อจบประโยค ---
        if time.time() - last_voice_time > SILENCE_DURATION:
            final = json.loads(rec.FinalResult()).get("text", "").strip()
            if final:
                print("\n[SENTENCE]", final)
            rec.Reset()
            continue

        # partial (ไม่ใช่การตัดประโยค)
        partial = json.loads(rec.PartialResult()).get("partial", "").strip()
        if partial:
            print("[PARTIAL]", partial)

        time.sleep(0.005)
