import { DetailComponent } from './detail.component';
import { NoLifelineError } from './no_lifeline/no_lifeline.component';
import { Routes } from '@angular/router';

export const error_routes: Routes = [
  // { path: '', children: [
    // { path: '', component: DetailComponent },
    { path: 'no-lifeline', component: NoLifelineError }
  // ]},
];