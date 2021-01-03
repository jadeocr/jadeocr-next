var mongoose = require('mongoose')

var classSchema = new mongoose.Schema({
    teacherId: String,
    teacherName: String,
    name: String,
    description: String,
    students: Array,
    classCode: String,
    assignedDecks: [{
        deckId: String,
        mode: {
            type: String,
            enum: ["learn", "srs", "quiz"],
        },
        handwriting: Boolean,
        front: {
            type: String,
            enum: ["character", "pinyin", "definition", "handwriting"]
        },
        scramble: Boolean,
        repetitions: Number,
        results: {type: Object, default: {}},
    }],
}, {minimize: false})

module.exports = new mongoose.model('classes', classSchema)