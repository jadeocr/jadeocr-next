var hanziLookup = require('../public/hanzilookup/hanzilookup.min.js')
var mmah = require('../public/hanzilookup/mmah.json')

exports.get = function(req, res, next) {
  hanziLookup.init('mmah', JSON.stringify(mmah) , fileLoaded)

  function fileLoaded(arg) {
    if (!arg) {console.log('ocr database loading failed')}
  }

  var lineArray = JSON.parse(Object.keys(req.body)[0])

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
