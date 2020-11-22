import axios from 'axios'
const apiBaseURL = 'http://localhost:3000/api'

interface Deck {
  characters: Array<string>
  readonly id: string
  title: string
  description: string
  readonly creatorID: string
  readonly creatorFirst: string
  readonly creatorLast: string
  isPublic: false
}

export const decks = {
  namespaced: true,
  state: {
    decks: Array<Deck>(),
    decksAssigned: Array<Deck>(),
  },
  mutations: {
    // eslint-disable-next-line
    setDecks(state: any, decks:Array<Deck>) {
      state.decks = decks
    },
    // eslint-disable-next-line
    setAssignedDecks(state: any, decksAssigned:Array<Deck>) {
      state.decksAssigned = decksAssigned
    },
  },
  actions: {
    fetchDecks({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        url: `${apiBaseURL}/deck/decks`,
        withCredentials: true,
      })
        .then((res) => {
          commit('setDecks', res.data)
        })
        .catch((err) => {
          console.log(err)
        })
    },
    fetchAssignedDecks({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        url: `${apiBaseURL}/deck/assigned`,
        withCredentials: true,
      })
        .then((res) => {
          console.log(res) // TODO: Update state with value
        })
        .catch((err) => {
          console.log(err)
        })
    },
  },
}
