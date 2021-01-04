var mongoose = require('mongoose')

var userDetailedSchema = new mongoose.Schema({
    id: String,
    email: String,
    firstName: String,
    lastName: String,
    password: String,
    isTeacher: Boolean,
    classes: Array,
    classesTeaching: Array,
    stats: Array,
    decks: [{
        deckId: String,
        deckName: String,
        deckDescription: String,
        latestAccessDate: Number,
        totalQuizAttempts: Number,
        isOwner: { type: Boolean, default: false},
        learned: { type: Boolean, default: false},
        quizAttempts: [{
            attempt: Number,
            summary: {
                correct: Number,
                total: Number,
                overriden: Number,
            },
            stats: [{
                char: String,
                charId: String,
                correct: Boolean,
                overriden: Boolean,
            }]
        }],
        srs: [{
            charId: String,
            nextDue: Number,
            interval: Number,
            repetitions: Number,
            easiness: Number,
        }]
    }],
})

module.exports = mongoose.model('userDetailed', userDetailedSchema)