const spawn= require('child_process').spawn

exports.ocr = function(req, res, next) {
    var strokes = req.body.strokes || "[[(75,128),(83,172),(85,196)],[(84,127),(91,124),(123,121),(151,121),(161,121),(161,128),(165,157),(164,181),(162,195)],[(89,187),(103,194),(129,200),(149,199)]]"

    var ocrProcess = spawn('python', [__dirname + '/ocr.py',
                         strokes
                    ])

    ocrProcess.stderr.pipe(process.stderr)

    ocrProcess.stdout.on('data', function(data) {
        var chars = new Array()
        data = data.toString().split("\n")
        for (let i = 0; i < data.length - 1; i++) {
            chars.push(String.fromCharCode(parseInt(data[i])))
        }
        res.send(chars) 
    })
}