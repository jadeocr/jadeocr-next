import { Character } from './Character'
import { Access } from './Access'

export interface Deck {
  characters: Array<Character>
  readonly _id: string
  title: string
  description: string
  readonly creatorID: string
  readonly creatorFirst: string
  readonly creatorLast: string
  access: Access
}

