export interface Character {
  // readonly _id: string // Mongo generated
  readonly id: string // UUID generated, need to change server to use _id later
  char: string
  definition: string
  pinyin: string
}

