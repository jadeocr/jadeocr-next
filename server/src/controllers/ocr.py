from cnn_chinese_hw.recognizer.TFLiteRecognizer import TFLiteRecognizer
import sys
import ast

rec = TFLiteRecognizer()
# strokes = ast.literal_eval(sys.argv[1])

from flask import Flask
from flask import request
app = Flask(__name__)

@app.route('/', methods=['POST'])
def infer():
    strokes = request.form['char']
    data = rec.get_L_candidates(strokes,8)
    return data


# "[[(75,128),(83,172),(85,196)],[(84,127),(91,124),(123,121),(151,121),(161,121),(161,128),(165,157),(164,181),(162,195)],[(89,187),(103,194),(129,200),(149,199)]]"

# for i in data:
#     print(i[1])

# print(sys.argv)
# print(strokes)
# sys.stdout.flush()

if __name__ == '__main__':
    app.run()