import { createStore } from 'vuex'
import { auth } from './auth'
import { decks } from './decks'
import { classes } from './classes'

export default createStore({
  state: {},
  mutations: {},
  actions: {},
  modules: {
    auth,
    decks,
    classes,
  },
})
