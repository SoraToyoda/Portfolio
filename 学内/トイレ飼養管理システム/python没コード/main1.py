from flask import Flask, render_template
import serial
import time
import random

app = Flask(__name__)

last_status = '空き'
last_change_time = time.time()
toilet_paper = '十分'
last_status_random = '空き'
last_change_time_random = time.time()
toilet_paper_random = '十分'

# トイレットペーパーの残量を取得
def get_toilet_paper_status():
    ser = serial.Serial('COM8', 9600)
    value = int(ser.readline().decode().strip())
    ser.close()
    if value >= 1100:
        return '少ない'
    else:
        return '十分'

# トイレの使用状況を取得
def get_toilet_status():
    ser = serial.Serial('COM9', 9600)
    value = int(ser.readline().decode().strip())
    ser.close()
    if value <= 50:
        return '使用中'
    else:
        return '空き'

# トイレットペーパーの残量を取得（モック）
def get_toilet_paper_status_random():
    # ランダムな値を生成してテスト
    value = random.randint(1100, 1200)
    if value >= 1100:
        return '少ない'
    else:
        return '十分'

# トイレの使用状況を取得（モック）
def get_toilet_status_random():
    # ランダムな値を生成してテスト
    value = random.randint(0, 60)
    if value <= 50:
        return '使用中'
    else:
        return '空き'

# トイレの使用状況を取得
@app.route('/')
def index():
    global last_status, last_change_time, toilet_paper

    time.sleep(1)
    status = get_toilet_status()
    current_time = time.time()

    if last_status == '使用中' and status == '空き' and (current_time - last_change_time) >= 5:
        toilet_paper = get_toilet_paper_status()

    if last_status != status:
        last_change_time = current_time
        last_status = status

    use_time = current_time - last_change_time

    if status == '使用中':
        if use_time < 1200:  # 20分未満
            color = 'yellow'
        else:
            color = 'red'
    else:
        color = 'green'

    return render_template('index.html', toilet_paper=toilet_paper, status=status, color=color)

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5000)