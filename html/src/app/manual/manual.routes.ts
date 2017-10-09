import { DetailComponent } from './detail.component';
import { NoLifelineError } from './no_lifeline/no_lifeline.component';
import { Routes } from '@angular/router';
import { ObserverTabsComponent } from './observer-tabs.component';
import { ManualComponent } from './manual.component';

export const manual_routes: Routes = [
    { path: '', component: ManualComponent }
];