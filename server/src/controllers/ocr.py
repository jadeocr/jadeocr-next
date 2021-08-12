from cnn_chinese_hw.recognizer.TFLiteRecognizer import TFLiteRecognizer
import json
from flask import Flask
from flask import request

app = Flask(__name__)

rec = TFLiteRecognizer()

@app.route('/', methods=['POST'])
def infer():
    strokes = json.loads(request.form['char'])
    print(strokes)
    for i in strokes:
        print(i)
    data = rec.get_L_candidates(strokes,8)
    sendData = []
    for i in data:
        sendData.append(chr(i[1]))
    return json.dumps(sendData)

# "[[(75,128),(83,172),(85,196)],[(84,127),(91,124),(123,121),(151,121),(161,121),(161,128),(165,157),(164,181),(162,195)],[(89,187),(103,194),(129,200),(149,199)]]"

if __name__ == '__main__':
    app.run()