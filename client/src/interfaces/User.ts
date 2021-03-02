export interface User {
  email: string
  firstName: string
  lastName: string
  isTeacher: boolean
  readonly password: string
  readonly _id: string
}
