var mongoose = require('mongoose')

var deckSchema = new mongoose.Schema({
    title: String,
    description: String,
    characters: [{
        char: String,
        pinyin: String,
        definition: String,
        id: String
    }],
    creator: String,
    creatorFirst: String,
    creatorLast: String,
    createdDate: Number,
    access: {
        isPublic: Boolean,
        users: {type: Object, default: {}},
        classes: {type: Object, default: {}},
    },
}, {minimize: false})

module.exports = new mongoose.model("decks", deckSchema)