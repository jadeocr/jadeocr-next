const axios = require('axios')
const Form = require('form-data')

exports.ocr = function(req, res, next) {
  // var strokes = req.body.strokes || "[[(75,128),(83,172),(85,196)],[(84,127),(91,124),(123,121),(151,121),(161,121),(161,128),(165,157),(164,181),(162,195)],[(89,187),(103,194),(129,200),(149,199)]]"
  var strokes = req.body.strokes
  if (!strokes) {
    res.sendStatus(400)
    return
  } else {
    var formData = new Form()
    formData.append('char', strokes)
    axios.post('http://localhost:5000/', formData, {
      headers: formData.getHeaders()
    })
    .then(char => {
      res.send(char.data)
    })
    .catch(err => {
      console.log(err)
    })
  }
}
