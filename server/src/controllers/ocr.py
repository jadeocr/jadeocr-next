from cnn_chinese_hw.recognizer.TFLiteRecognizer import TFLiteRecognizer
import sys
import ast

rec = TFLiteRecognizer()
strokes = ast.literal_eval(sys.argv[1])

data = rec.get_L_candidates(strokes,8)

# "[[(75,128),(83,172),(85,196)],[(84,127),(91,124),(123,121),(151,121),(161,121),(161,128),(165,157),(164,181),(162,195)],[(89,187),(103,194),(129,200),(149,199)]]"

print(rec.get_L_candidates("[[(75,128),(83,172),(85,196)],[(84,127),(91,124),(123,121),(151,121),(161,121),(161,128),(165,157),(164,181),(162,195)],[(89,187),(103,194),(129,200),(149,199)]]", 8))
print(rec.get_L_candidates("[[(75,128),(83,172),(85,196)],[(84,127),(91,124),(123,121),(151,121),(161,121),(161,128),(165,157),(164,181),(162,195)],[(89,187),(103,194),(129,200),(149,199)]]", 8))

# toReturn = []

for i in data:
    # print((i[0], chr(i[1])))
    print(i[1])

# print(sys.argv)
# print(strokes)
sys.stdout.flush()