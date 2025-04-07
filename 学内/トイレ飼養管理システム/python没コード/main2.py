from flask import Flask, render_template
import serial
import time

app = Flask(__name__)

last_status = '空き'
last_change_time = time.time()
toilet_paper = '十分'

# シリアルポートの初期化
ser1 = serial.Serial('COM8', 9600, timeout=1)
ser2 = serial.Serial('COM9', 9600, timeout=1)

# トイレットペーパーの残量を取得
def get_toilet_paper_status():
    line = ser1.readline().strip()
    if line:
        value = int(line.decode('utf-8', errors='ignore'))
        if value >= 1100:
            return '少ない'
        else:
            return '十分'
    return '不明'

# トイレの使用状況を取得
def get_toilet_status():
    line = ser2.readline().strip()
    if line:
        value = int(line.decode('utf-8', errors='ignore'))
        if value <= 50:
            return '使用中'
        else:
            return '空き'
    return '不明'

# メインの処理
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
        color = '#90ee90'

    return render_template('index.html', toilet_paper=toilet_paper, status=status, color=color)

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5000)