var mongoose = require('mongoose')

var svgSchema = new mongoose.Schema({
    character: String,
    svg: String,
}, {collection: "staticsvgs"})

module.exports = mongoose.model("staticsvgs", svgSchema)