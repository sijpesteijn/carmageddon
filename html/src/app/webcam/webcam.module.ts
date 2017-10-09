import { NgModule } from '@angular/core';
import { StreamComponent } from './stream/stream.component';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { WebcamComponent } from './webcam.component';
@NgModule({
    imports: [
        CommonModule,
        FormsModule
    ],
    exports: [
        WebcamComponent
    ],
    declarations: [
        StreamComponent,
        WebcamComponent
    ]
})
export class WebcamModule {

}