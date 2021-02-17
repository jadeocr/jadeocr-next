export interface AssignedDeck {
  // Data from getAssignedDecks API
  assignedDate: string
  deckDescription: string
  deckId: string
  deckName: string
  dueDate: number
  front: string
  handwriting: string
  mode: string
  repetitions: number
  scramble: boolean
  status: string
  _id: string // id of unique assignment (assignmentId)
}
