import { Component, ChangeDetectionStrategy } from '@angular/core';
import { ImageManipulationService } from './services/image-manipulation/image-manipulation.service';
import { Image } from './models/image';
import Swal from 'sweetalert2';


@Component({
  selector: 'app-root',
  standalone: false,
  templateUrl: './app.component.html',
  changeDetection: ChangeDetectionStrategy.Eager,
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  private readonly fileReader = new FileReader();
  // Image to process
  public img?: Image;
  public loading: boolean = false;
  // Image to show
  public source: string;
  // App title
  public title: string = 'Image Filter and Classifier';

  constructor(private imgManipulationService: ImageManipulationService) {
    this.source = 'https://pngimage.net/wp-content/uploads/2018/06/lena-png-3.png'
  }

  public onChange(event: Event): void {
    const files = (event.target as HTMLInputElement).files;
    this.loading = true;

    if (files?.length) {
      this.fileReader.onload = () => {
        const data = this.fileReader.result;
        if (typeof data !== 'string') {
          this.loading = false;
          return;
        }

        this.img = {
          name: 'Lena.png',
          source: data,
          data
        }
        this.loading = false;
      };
  
      this.fileReader.readAsDataURL(files[0]);
    }
  };

  public onProcessImage(): void {
    if (this.img) {
      const response = this.imgManipulationService.filterImage(this.img);
      response.subscribe((text) => {
        Swal.fire('Image was sent', 'Image was processing correctly.', 'info');
      }, (error) => {
        Swal.fire('Error!', 'Image wasn\'t sent', 'error');
      });
    } else {
      Swal.fire('Image does\'nt selected.', 'You must select an image.', 'warning');
    }
  }
}
