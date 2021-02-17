import { Character } from './Character'
import { Access } from './Access'

export interface Deck {
  characters: Array<Character>
  title: string
  description: string
  access: Access
  readonly creatorID?: string
  readonly creatorFirst?: string
  readonly creatorLast?: string
  readonly _id: string
}
