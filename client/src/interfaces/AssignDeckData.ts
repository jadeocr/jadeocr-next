export interface AssignDeckData {
  // Used when calling the assignment API
  mode: string
  front: string
  scramble: null | boolean
  repetitions: number
  dueDate: string | Date | number
  classCode: string
  deckId: string
}
