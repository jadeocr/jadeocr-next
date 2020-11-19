import { createStore } from 'vuex'
import { auth } from './auth'
import { decks } from './decks'

export default createStore({
  state: {},
  mutations: {},
  actions: {},
  modules: {
    auth,
    decks,
  },
})
