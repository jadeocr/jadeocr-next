var characterModel = require('../models/characterModel')
var strokeModel = require('../models/strokeModel')

exports.pinyin = function(req, res, next) {
    let character = req.body.character

    characterModel.findOne({character: character}, function(err, char) {
        if (err) console.log(err)
        if (!char) {
            res.send("No matches found")
        } else {
            res.send(char.pinyin)
            console.log(char)
        }
    })
}

exports.definition = function(req, res, next) {
    let character = req.body.character

    characterModel.findOne({character: character}, function(err, char) {
        if (err) console.log(err)
        if (!char) {
            res.send("No matches found")
        } else {
            res.send(char.definition)
        }
    })
}