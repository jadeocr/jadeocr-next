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
  readonly deckId: string
  deckName: string
  deckDescription: string
  readonly creatorID: string
  readonly creatorFirst: string
  readonly creatorLast: string
  isPublic: boolean
}

export const decks = {
  namespaced: true,
  state: {
    decks: Array<Deck>(),
    decksAssigned: Array<Deck>(),
    decksErrorMsg: '',
  },
  getters: {
    // eslint-disable-next-line
    getDeck(state: any, id: string): Deck {
      return state.decks.decks.find((deck: Deck) => {
        return deck.deckId == id
      })
    },
  },
  mutations: {
    // eslint-disable-next-line
    setDecks(state: any, decks: Array<Deck>) {
      state.decks = decks
    },
    // eslint-disable-next-line
    setAssignedDecks(state: any, decksAssigned: Array<Deck>) {
      state.decksAssigned = decksAssigned
    },
    // eslint-disable-next-line
    setDeckErrMsg(state: any, msg: string) {
      state.decksErrorMsg = msg
    },
  },
  actions: {
    fetchAllDecks({ commit }: { commit: Function }): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/getUsedDecks`,
        withCredentials: true,
      })
        .then((res) => {
          console.log(res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    fetchCreatedDecks({ commit }: { commit: Function }): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/createdDecks`,
        withCredentials: true,
      })
        .then((res) => {
          console.log(res)
          commit('setDecks', res.data) // TODO: Update mutation name
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    fetchAssignedDecks({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        url: `${apiBaseURL}/deck/assigned`,
        withCredentials: true,
      })
        .then((res) => {
          // console.log(res) // TODO: Update state with value
          commit('setAssignedDecks', res.data)
        })
        .catch(() => { // User is not enrolled in any classes, can return
          return
        })
    },
    // TODO: Error handling
    createDeck({ commit }: { commit: Function }, deck: Deck): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/create`,
        withCredentials: true,
        data: {
          deckName: deck.deckName,
          description: deck.deckDescription, // inconsistency between description and deckDescription
          characters: deck.characters,
          isPublic: deck.isPublic,
        },
      })
        .then(() => {
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          const msg: string = err.response.data.errors[0].msg
          if (msg == 'title') {
            commit('setDeckErrMsg', 'Missing title')
          } else {
            commit('setDeckErrMsg', msg)
          }
          console.log(err.response.data)
        })
    },
    updateDeck({ commit }: { commit: Function }, deck: Deck): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/update`,
        withCredentials: true,
        data: {
          deckId: deck.deckId,
          deckName: deck.deckName,
          description: deck.deckDescription,
          characters: deck.characters,
          isPublic: deck.isPublic,
        },
      })
        .then(() => {
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          const msg: string = err.response.data.errors[0].msg
          if (msg == 'title') {
            commit('setDeckErrMsg', 'Missing title')
          } else {
            commit('setDeckErrMsg', msg)
          }
          console.log(err.response.data)
        })
    },
    deleteDeck({ commit }: { commit: Function }, deck: Deck): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/delete`,
        withCredentials: true,
        data: {
          deckId: deck.deckId,
        },
      })
        .then(() => {
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    getPinyinDefinition({ commit }: { commit: Function }, payload: {
      deck: Deck,
      charIndex: number
    }): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/pinyinAndDefinition`,
        data: {
          'character': payload.deck.characters[payload.charIndex].char
        }
      })
      .then((res: any) => {
        const currChar = payload.deck.characters[payload.charIndex]
        currChar.pinyin = res.data.pinyin.join(', ')
        currChar.definition = res.data.definition
      })
      .catch((err) => {
        console.log(err.response.data)
      })
    }
  },
}
