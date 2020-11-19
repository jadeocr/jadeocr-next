import axios from 'axios'
const apiBaseURL = 'http://localhost:3000/api'

interface Deck {
  characters: Array<string>,
  readonly id: string,
  title: string,
  description: string,
  readonly creatorID: string,
  readonly creatorFirst: string,
  readonly creatorLast: string,
  isPublic: false
}


export const decks = {
  namespaced: true,
  state: {
    decks: Array<Deck>(),
  },
  mutations: {
    // eslint-disable-next-line
    setDecks(state: any, decks:Array<Deck>) {
      state.decks = decks
    }
  },
  actions: {
    fetchDecks({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        url: `${apiBaseURL}/deck/decks`,
      })
        .then((res) => {
          console.log(res)
        })
        .catch((err) => {
          console.log(err)
        })
    },
    fetchAssignedDecks({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        url: `${apiBaseURL}/deck/assigned`,
      })
        .then((res) => {
          console.log(res.config.data)
        })
        .catch((err) => {
          console.log(err)
        })
    },
  }
}
