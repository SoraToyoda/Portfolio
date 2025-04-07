import cv2
from tensorflow.keras.models import load_model
import numpy as np
import time
import matplotlib.pyplot as plt
from datetime import datetime

# モデルのロード
model = load_model('emotion_model.h5')

# カメラの起動
cap = cv2.VideoCapture(1)

# 顔認識のためのカスケード分類器
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# 起動時刻の記録
start_time = time.time()
emotion_seconds = {emotion: 0.0 for emotion in ['Anger', 'Disgust', 'Fear', 'Happy', 'Sad', 'Surprise', 'Neutral']}

while True:
    frame_start_time = time.time()  # フレームの開始時刻を記録

    ret, frame = cap.read()
    if not ret:
        break

    # 左右反転（内側カメラ用）
    frame = cv2.flip(frame, 1)

    # 現在の経過秒数
    elapsed_time = time.time() - start_time

    # 顔検出
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    detected_emotions = set()  # このフレームで検出された表情を記録
    for (x, y, w, h) in faces:
        # 顔部分を切り取る
        face = frame[y:y+h, x:x+w]
        face = cv2.cvtColor(face, cv2.COLOR_BGR2GRAY)
        face = cv2.resize(face, (48, 48))
        face = np.reshape(face, (1, 48, 48, 1))  # バッチサイズを1に設定
        face = face / 255.0

        # 感情予測
        emotion = model.predict(face)
        max_index = np.argmax(emotion[0])
        emotion_label = ['Anger', 'Disgust', 'Fear', 'Happy', 'Sad', 'Surprise', 'Neutral'][max_index]

        # 検出された表情を記録
        detected_emotions.add(emotion_label)

        # 結果を画面に表示
        cv2.putText(frame, emotion_label, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    # 各表情の秒数を更新
    frame_time = time.time() - frame_start_time
    for emotion in detected_emotions:
        emotion_seconds[emotion] += frame_time

    # 経過秒数と各感情の秒数をウィンドウに表示
    y_offset = 30
    cv2.putText(frame, f"Elapsed Time: {elapsed_time:.2f} sec", (10, y_offset), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
    y_offset += 30
    for emotion, seconds in emotion_seconds.items():
        cv2.putText(frame, f"{emotion}: {seconds:.2f} sec", (10, y_offset), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
        y_offset += 30

    cv2.imshow("Emotion Detection", frame)

    # 'q' キーが押されたら終了
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 総経過時間
total_time = time.time() - start_time

# 棒グラフと円グラフを生成
timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
output_file = f"emotion_analysis_{timestamp}.png"

# emotion_label の順序に基づいてデータを並べ替え
emotion_order = ['Neutral', 'Happy', 'Anger', 'Fear', 'Sad', 'Surprise', 'Disgust']
sorted_data_bar = sorted(emotion_seconds.items(), key=lambda x: emotion_order.index(x[0]))
labels_bar, sizes_bar = zip(*sorted_data_bar)

# 円グラフのデータを割合が小さい順に並べ替え
sorted_data_pie = sorted(emotion_seconds.items(), key=lambda x: x[1])  # 小さい順に並べ替え
labels_pie, sizes_pie = zip(*sorted_data_pie)

# グラフの作成
fig, axes = plt.subplots(1, 2, figsize=(16, 8))  # 横並びに2つのプロット

# 棒グラフ
ax_bar = axes[0]
bars = ax_bar.bar(labels_bar, sizes_bar, color='skyblue')

# 各バーに秒数と割合（％）を表示
for bar, size in zip(bars, sizes_bar):
    percentage = (size / total_time) * 100 if total_time > 0 else 0
    ax_bar.text(bar.get_x() + bar.get_width() / 2, bar.get_height() + 0.1,
                f"{size:.2f} sec ({percentage:.2f}%)",
                ha='center', va='bottom', fontsize=10)

# 棒グラフの体裁を整える
ax_bar.set_title('Emotion Analysis (Bar Chart)', fontsize=16)
ax_bar.set_ylabel('Time (seconds)', fontsize=14)
ax_bar.set_xlabel('Emotions', fontsize=14)
ax_bar.set_ylim(0, max(sizes_bar) + 5)
ax_bar.tick_params(axis='x', rotation=45)

# 総合計時間を左上に追加
ax_bar.text(0.1, 0.85, f"total time: {total_time:.2f} sec",
        transform=ax_bar.transAxes, fontsize=15, va='top', ha='left',
        bbox=dict(boxstyle="round,pad=1", edgecolor="black", facecolor="white"))

# 円グラフ
colors = plt.cm.Paired(range(len(labels_pie)))
ax_pie = axes[1]
ax_pie.pie(sizes_pie, labels=labels_pie, autopct=lambda p: f'{p:.2f}%' if p > 0 else '',
           colors=colors, startangle=90, wedgeprops={'edgecolor': 'black'})
ax_pie.set_title('Emotion Analysis (Pie Chart)', fontsize=16)

plt.tight_layout()

# ファイルに保存
plt.savefig(output_file)
plt.close()

# 結果の表示
print(f"Total elapsed time: {total_time:.2f} seconds")
for emotion, time_sec in emotion_seconds.items():
    percentage = (time_sec / total_time) * 100 if total_time > 0 else 0
    print(f"{emotion}: {time_sec:.2f} seconds ({percentage:.2f}%)")

print(f"Graph saved as: {output_file}")

cap.release()
cv2.destroyAllWindows()
