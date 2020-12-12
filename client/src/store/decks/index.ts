import router from '../../router/index'
import axios from 'axios'
const apiBaseURL = process.env.VUE_APP_API_BASEURL

interface Character {
  // readonly _id: string // Mongo generated
  readonly id: string // UUID generated, need to change server to use _id later 
  char: string
  definition: string
  pinyin: string
}

interface Deck {
  characters: Array<Character>
  readonly _id: string
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
  getters: {
    getDeck(state: any, id:string): Deck {
      return state.decks.decks.find(( deck: Deck ) => {
        return deck._id == id
      })
    }
  },
  mutations: {
    setDecks(state: any, decks: Array<Deck>) {
      state.decks = decks
    },
    setAssignedDecks(state: any, decksAssigned: Array<Deck>) {
      state.decksAssigned = decksAssigned
    },
  },
  actions: {
    fetchDecks({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        url: `${apiBaseURL}/deck/mydecks`,
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
    // TODO: Error handling
    createDeck({ commit }: { commit: Function }, deck: Deck): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/create`,
        withCredentials: true,
        data: {
          title: deck.title,
          description: deck.description,
          characters: deck.characters,
          isPublic: deck.isPublic
        }
      })
      .then(() => {
        router.push({ name: 'Dashboard' })
      })
      .catch((err) => {
        console.log(err)
      })
    },
    updateDeck({ commit }: { commit: Function }, deck: Deck): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/update`,
        withCredentials: true,
        data: {
          deckId: deck._id,
          title: deck.title,
          description: deck.description,
          characters: deck.characters,
          isPublic: deck.isPublic,
        }
      })
      .then(() => {
        router.push({ name: 'Dashboard' })
      })
      .catch((err) => {
        console.log(err)
      })
    },
    deleteDeck({ commit }: { commit: Function }, deck: Deck): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/delete`,
        withCredentials: true,
        data: {
          deckId: deck._id,
        }
      })
      .then(() => {
        router.push({ name: 'Dashboard' })
      })
      .catch((err) => {
        console.log(err)
      })
    },
  },
}
