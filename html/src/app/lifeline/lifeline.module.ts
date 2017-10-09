import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { LifeLineComponent } from './lifeline.component';
import { RouterModule } from '@angular/router';
@NgModule({
    imports: [
        CommonModule,
        RouterModule
    ],
    exports: [
        LifeLineComponent
    ],
    declarations: [
        LifeLineComponent
    ]
})
export class LifeLineModule {

}