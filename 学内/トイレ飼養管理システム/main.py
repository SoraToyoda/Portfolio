from flask import Flask, render_template
import serial
import time

app = Flask(__name__)

# グローバル変数の初期化
last_status = '空き'
last_change_time = time.time()
toilet_paper = '十分'


# トイレットペーパーの残量を判定
def toilet_paper_status():
    ser = serial.Serial('COM8', 9600) # COMポートの設定
    value = int(ser.readline().decode().strip()) # 値を取得
    ser.close() # COMポートを閉じる
    if value >= 900: # 判定
        return '少ない'
    else:
        return '十分'

# トイレの使用状況を判定
def toilet_status():
    ser = serial.Serial('COM9', 9600) # COMポートの設定
    value = int(ser.readline().decode().strip()) # 値を取得
    ser.close() # COMポートを閉じる
    if value <= 50: # 判定
        return '使用中'
    else:
        return '空き'

# mainの処理
@app.route('/')
def index():
    global last_status, last_change_time, toilet_paper

    time.sleep(1)
    status = toilet_status()
    current_time = time.time()

    # トイレットペーパーの残量を更新
    if last_status == '使用中' and status == '空き' and (current_time - last_change_time) >= 5:
        toilet_paper = toilet_paper_status()

    # トイレの使用状況を更新
    if last_status != status:
        last_change_time = current_time
        last_status = status

    # 使用時間を計算
    use_time = current_time - last_change_time

    # 使用時間を基に色を設定
    if status == '使用中':
        if use_time < 5:  # 20分未満
            color = 'yellow'
        else:
            color = '#ff7f7f'
    else:
        color = '#90ee90'

    return render_template('index.html', toilet_paper=toilet_paper, status=status, color=color)

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5000)