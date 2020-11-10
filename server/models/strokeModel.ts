var mongoose = require('mongoose')

var strokeSchema = new mongoose.Schema({
    character: String,
    strokes: Array,
    medians: Array
}, {collection: "graphics"})

module.exports = new mongoose.model("strokes", strokeSchema)