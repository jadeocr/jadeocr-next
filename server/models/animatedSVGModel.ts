var mongoose = require('mongoose')

var svgSchema = new mongoose.Schema({
    character: String,
    svg: String,
}, {collection: "animatedsvgs"})

module.exports = mongoose.model("animatedsvgs", svgSchema)