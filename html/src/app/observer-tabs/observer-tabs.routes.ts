import { DetailComponent } from './detail.component';
import { NoLifelineError } from './no_lifeline/no_lifeline.component';
import { Routes } from '@angular/router';
import { ObserverTabsComponent } from './observer-tabs.component';

export const observer_tabs_routes: Routes = [
    { path: '', component: ObserverTabsComponent },
    { path: 'race', component: ObserverTabsComponent }
];