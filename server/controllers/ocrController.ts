#! python3

var hanziLookup = require('../public/hanzilookup/hanzilookup.min.js')
var mmah = require('../public/hanzilookup/mmah.json')

exports.post = function(req, res, next) {
  hanziLookup.init('mmah', JSON.stringify(mmah) , fileLoaded)

  function fileLoaded(arg) {
    if (!arg) {console.log('ocr database loading failed')}
  }

  var lineArray = []
  if (JSON.parse(Object.keys(req.body)[0])) {
    lineArray = JSON.parse(Object.keys(req.body)[0])
  } else {
    lineArray = [[[75,128],[83,172],[85,196]],[[84,127],[91,124],[123,121],[151,121],[161,121],[161,128],[165,157],[164,181],[162,195]],[[89,187],[103,194],[129,200],[149,199]]]
    // gives character 口
  }


  function lookup(strokeArray) {
    try {
      var analyzedChar = new hanziLookup.AnalyzedCharacter(strokeArray)
      var matcherMMAH = new hanziLookup.Matcher('mmah')
      matcherMMAH.match(analyzedChar, 8, function(matches) {
        res.send(matches[0].character)
        console.log(matches)
      })
    } catch(err) {
      if (err.name == 'TypeError') {
        res.send('No matches were found')
      }
    } 
  }

  lookup(lineArray)
}
