import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { ObserverTabsComponent } from './observer-tabs.component';
import { ObserverModule } from '../observers/observer.module';
import { FormsModule } from '@angular/forms';
import { RouterModule } from '@angular/router';
import { observer_tabs_routes } from './observer-tabs.routes';
@NgModule({
    imports: [
        CommonModule,
        FormsModule,
        ObserverModule,
        RouterModule.forChild(observer_tabs_routes)
    ],
    exports: [
        ObserverTabsComponent
    ],
    declarations: [
        ObserverTabsComponent
    ]
})
export class ObserverTabsModule {

}