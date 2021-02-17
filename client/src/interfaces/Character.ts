export interface Character {
  // readonly _id: string // Mongo generated
  readonly id?: string // TODO: UUID generated, need to change server to use _id
  char: string
  definition: string
  pinyin: string
}
