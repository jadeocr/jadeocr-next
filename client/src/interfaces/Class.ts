export interface ClassI {
  classCode: string
  description: string
  name: string
  readonly _id?: string
  readonly teacherId?: string
  teacherName: string
  students: Array<any>
  assignedDecks: Array<any>
}
