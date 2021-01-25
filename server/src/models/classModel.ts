var mongoose = require('mongoose')

var classSchema = new mongoose.Schema({
    teacherId: String,
    teacherName: String,
    name: String,
    description: String,
    students: [{
        firstName: String,
        lastName: String,
        id: String,
    }],
    classCode: String,
    assignedDecks: [{
        deckId: String,
        deckName: String,
        deckDescription: String,
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
        assignedDate: Number,
        dueDate: Number,
        results: {type: Object, default: {}},
    }],
}, {
    minimize: false,
    strict: false,
})

module.exports = new mongoose.model('classes', classSchema)