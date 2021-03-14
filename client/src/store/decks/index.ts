const apiBaseURL = process.env.VUE_APP_API_BASEURL
import axios from 'axios'
import { AxiosResponse } from 'axios'
import router from '../../router/index'
import { Deck } from '../../interfaces/Deck'
import { Character } from '../../interfaces/Character'
import { ReviewResult } from '../../interfaces/ReviewResult'
import { PinyinDefinition } from '../../interfaces/PinyinDefinition'

export const decks = {
  namespaced: true,
  state: {
    decks: Array<Deck>(),
    currDeck: {} as Deck,
    decksAssigned: Array<Deck>(),
    decksCreated: Array<Deck>(),
    decksErrorMsg: '',
    publicDecks: Array<Deck>(),
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
    setCreatedDecks(state: any, decksCreated: Array<Deck>) {
      state.decksCreated = decksCreated
    },
    // eslint-disable-next-line
    setDeckErrMsg(state: any, msg: string) {
      state.decksErrorMsg = msg
    },
    // eslint-disable-next-line
    setPublicDecks(state: any, publicDecks: Array<Deck>) {
      state.publicDecks = publicDecks
    },
  },
  actions: {
    async fetchDecks({ commit }: { commit: Function }): Promise<any> {
      return axios({
        method: 'post',
        url: `${apiBaseURL}/deck/allDecks`,
        withCredentials: true,
      })
        .then((res: AxiosResponse<Array<Deck>>) => {
          commit('setDecks', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    async fetchAssignedDecks({ commit }: { commit: Function }): Promise<any> {
      return axios({
        method: 'get',
        url: `${apiBaseURL}/deck/assigned`,
        withCredentials: true,
      })
        .then((res: AxiosResponse<Array<Deck>>) => {
          commit('setAssignedDecks', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    async fetchCreatedDecks({ commit }: { commit: Function }): Promise<any> {
      return axios({
        method: 'post',
        url: `${apiBaseURL}/deck/createdDecks`,
        withCredentials: true,
      })
        .then((res: AxiosResponse<Array<Deck>>) => {
          commit('setCreatedDecks', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    async fetchCards(
      { commit }: { commit: Function },
      deckId: string
    ): Promise<any> {
      return axios({
        method: 'post',
        url: `${apiBaseURL}/deck/deck`,
        withCredentials: true,
        data: {
          deckId: deckId,
        },
      })
        .then((res: AxiosResponse) => {
          commit('setCurrDeck', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
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
    // eslint-disable-next-line
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
      // eslint-disable-next-line
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
        .then((res: AxiosResponse<PinyinDefinition>) => {
          const currChar = payload.deck.characters[payload.charIndex]
          currChar.pinyin = res.data.pinyin.join(', ')
          currChar.definition = res.data.definition
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    sendReviewResults(
      // eslint-disable-next-line
      { commit }: { commit: Function },
      payload: {
        deckId: string
        type: string
        results: Array<ReviewResult>
      }
    ): void {
      console.log(payload.deckId)
      if (payload.type == 'quiz') {
        axios({
          method: 'post',
          url: `${apiBaseURL}/deck/quizzed`,
          withCredentials: true,
          data: {
            deckId: payload.deckId,
            results: payload.results,
          },
        })
          .then(() => {
            router.push({ name: 'Deck', params: { id: payload.deckId } })
          })
          .catch((err) => {
            console.log(err.response.data)
          })
      } else {
        axios({
          method: 'post',
          url: `${apiBaseURL}/deck/practiced`,
          withCredentials: true,
          data: {
            deckId: payload.deckId,
            results: payload.results,
          },
        })
          .then(() => {
            router.push({ name: 'Deck', params: { id: payload.deckId } })
          })
          .catch((err) => {
            console.log(err.response.data)
          })
      }
    },
    publicFromQuery({ commit }: { commit: Function }, query: string): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/deck/public`,
        withCredentials: true,
        data: {
          query: query,
        },
      })
        .then((res: AxiosResponse<Array<Deck>>) => {
          commit('setPublicDecks', res.data)
        })
        .catch((err) => {
          console.log(err)
        })
    },
  },
}
