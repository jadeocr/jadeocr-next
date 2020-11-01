var mongoose = require('mongoose')

var deckScheme = new mongoose.Schema({
    title: String,
    description: String,
    characters: Array,
    creator: String,
    isPublic: Boolean,
})