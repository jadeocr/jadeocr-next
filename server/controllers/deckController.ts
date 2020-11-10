var deckModel = require('../models/deckModel')

exports.createDeck = function(req, res, next) {
    var deck = new deckModel({
        title: req.body.title,
        description: req.body.description,
        characters: req.body.characters,
        creator: req.user.id,
        isPublic: req.body.ispublic
    })
    deck.save(function(err) {
        if (err) {
            console.log(err)
            res.sendStatus(400)
        } else {
            res.sendStatus(200)
        }
    })
}

exports.findDecks = function(req, res, next) {
    deckModel.find({creator: req.user._id}, function(err, decks) {
        if (err) console.log(err) 
        res.send(decks)
    })
}