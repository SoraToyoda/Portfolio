import pandas as pd
import numpy as np
import cv2
from sklearn.preprocessing import LabelEncoder
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Conv2D, MaxPooling2D, Flatten, Dropout
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.utils import to_categorical
from tensorflow.keras.models import load_model

# CSVファイルを読み込む
data = pd.read_csv('C:/oit/python-3.10.8-embed-amd64/code/charenge/fer2013.csv')

# データの準備
X = []
y = []

# ピクセルデータとラベルをリストに格納
for index, row in data.iterrows():
    pixels = row['pixels'].split(' ')  # 空白で区切られたピクセル値
    image = np.array(pixels, dtype='float32')
    image = image.reshape(48, 48, 1)  # 48x48のグレースケール画像に変形
    X.append(image)
    y.append(row['emotion'])

X = np.array(X)
y = np.array(y)

# 画像データを正規化
X = X / 255.0

# ラベルをカテゴリカルにエンコード
label_encoder = LabelEncoder()
y = label_encoder.fit_transform(y)
y = to_categorical(y, num_classes=7)  # 感情ラベルは7クラス

# データの形状を確認
print(X.shape, y.shape)

# モデルの構築
model = Sequential()
model.add(Conv2D(64, (3, 3), activation='relu', input_shape=(48, 48, 1)))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Conv2D(128, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Flatten())
model.add(Dense(256, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(7, activation='softmax'))  # 7種類の感情分類

# モデルのコンパイル
model.compile(optimizer=Adam(), loss='categorical_crossentropy', metrics=['accuracy'])

# モデルの学習
model.fit(X, y, epochs=30, batch_size=64)

# モデルの保存
model.save('emotion_model.h5')

# モデルのロード（後で使用する場合）
# model = load_model('emotion_model.h5')

