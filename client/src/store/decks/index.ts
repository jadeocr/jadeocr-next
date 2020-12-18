const apiBaseURL = process.env.VUE_APP_API_BASEURL
import axios from 'axios'
import router from '../../router/index'
import { Deck } from '../../interfaces/Deck'
import { Character } from '../../interfaces/Character'
import { ReviewResult } from '../../interfaces/ReviewResult'

export const decks = {
  namespaced: true,
  state: {
    decks: Array<Deck>(),
    currDeck: {} as Deck,
    decksAssigned: Array<Deck>(),
    decksErrorMsg: '',
  },
  mutations: {
    // eslint-disable-next-line
    setDecks(state: any, decks: Array<Deck>) {
      state.decks = decks
    },
    // eslint-disable-next-line
    setCurrDeck(state: any, deck: Deck | Number) {
      state.currDeck =
        deck == -1
          ? {
              characters: Array<Character>(),
              title: '',
              description: '',
              access: {
                isPublic: false,
              },
            }
          : deck
    },
    // eslint-disable-next-line
    pushToCurrDeck(state: any) {
      state.currDeck.characters.push({
        char: '',
        definition: '',
        pinyin: '',
      } as Character)
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
    fetchDecks({ commit }: { commit: Function }): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/allDecks`,
        withCredentials: true,
      })
        .then((res) => {
          commit('setDecks', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    fetchCards({ commit }: { commit: Function }, deckId: string): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/deck`,
        withCredentials: true,
        data: {
          deckId: deckId,
        },
      })
        .then((res) => {
          commit('setCurrDeck', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
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
          description: deck.description, // inconsistency between description and description
          characters: deck.characters,
          isPublic: deck.access.isPublic,
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
          deckId: deck._id,
          title: deck.title,
          description: deck.description,
          characters: deck.characters,
          isPublic: deck.access.isPublic,
        },
      })
        .then(() => {
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          const msg: string = err.response.data
          if (msg == 'title') {
            commit('setDeckErrMsg', 'Missing title')
          } else {
            commit('setDeckErrMsg', msg)
          }
        })
    },
    deleteDeck({ commit }: { commit: Function }, deck: Deck): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/delete`,
        withCredentials: true,
        data: {
          deckId: deck._id,
        },
      })
        .then(() => {
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    getPinyinDefinition(
      { commit }: { commit: Function },
      payload: {
        deck: Deck
        charIndex: number
      }
    ): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/pinyinAndDefinition`,
        data: {
          character: payload.deck.characters[payload.charIndex].char,
        },
      })
        .then((res: any) => {
          const currChar = payload.deck.characters[payload.charIndex]
          currChar.pinyin = res.data.pinyin.join(', ')
          currChar.definition = res.data.definition
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    sendReviewResults({ commit }: { commit: Function }, results: Array<ReviewResult>): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/practiced`,
        withCredentials: true,
        data: {
          results: results
        }
      })
        .then((res) => {
          console.log(res)
        })
        .catch((err) => {
          console.log(err)
        })
    }
  },
}
