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
        totalQuizAttempts: Number,
        quizAttempts: [{
            attempt: Number,
            summary: {
                correct: Number,
                total: Number,
                overriden: Number,
            },
            stats: [{
                charId: String,
                correct: Boolean,
                overriden: Boolean,
            }]
        }],
        srs: [{
            charId: String,
            nextDue: Date,
            interval: Number,
            repetitions: Number,
            easiness: Number,
        }]
    }],
})

module.exports = mongoose.model('userDetailed', userDetailedSchema)